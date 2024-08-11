#include "vision_guard.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <utils/slog.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#endif

VisionGuard::VisionGuard(const std::string &gaze_model,
                         const std::string &face_model,
                         const std::string &head_pose_model,
                         const std::string &landmarks_model,
                         const std::string &eye_state_model,
                         const std::string &device)
    : start_time(std::chrono::steady_clock::now()),
      lastCheckTime(std::chrono::steady_clock::now()),
      gazeLostTime(std::chrono::steady_clock::now()),
      faceDetector(core, face_model, device, 0.5, false),
      headPoseEstimator(core, head_pose_model, device),
      landmarksEstimator(core, landmarks_model, device),
      eyeStateEstimator(core, eye_state_model, device),
      gazeEstimator(core, gaze_model, device),
      resultsMarker(false, false, false, true, true),
      presenter("", 650, cv::Size(320, 60)) {
  // Load OpenVINO runtime
  slog::info << ov::get_openvino_version() << slog::endl;
  estimators = {&headPoseEstimator, &landmarksEstimator, &eyeStateEstimator,
                &gazeEstimator};
  slog::info << "Available Devices:";
  slog::info << getAvailableDevices() << slog::endl;

  toggleStates = {{TOGGLE_ALL, false},
                  {TOGGLE_EYE_STATE, true},
                  {TOGGLE_FACE_BOUNDING_BOX, false},
                  {TOGGLE_GAZE, true},
                  {TOGGLE_HEAD_POSE_AXES, false},
                  {TOGGLE_LANDMARKS, false},
                  {TOGGLE_NONE, false},
                  {TOGGLE_RESOURCE_GRAPH, false}};
}

VisionGuard::~VisionGuard() {
  slog::debug << "Destroying VisionGuard Object" << slog::endl;
  slog::info << "Metrics report:" << slog::endl;
  metrics.logTotal();
  slog::info << presenter.reportMeans() << slog::endl;
  // Log gaze data and clean old data before destroying the object
  logGazeData();
  cleanOldData();
}

/**
 * @brief Checks if the accumulated gaze time has exceeded the threshold.
 * @return True if the accumulated gaze time has exceeded the threshold, false
 * otherwise.
 */
bool VisionGuard::checkGazeTimeExceeded() const {
  return accumulatedGazeTime >= accumulated_gaze_time_threshold;
}
/**
 * @brief Cleans old gaze data from the data file.
 */
void VisionGuard::cleanOldData() {
  auto now = std::chrono::system_clock::now();
  auto oneWeekAgo = now - std::chrono::hours(24 * 7);
  nlohmann::json data = readDataFile();

  for (auto it = data.begin(); it != data.end();) {
    std::istringstream ss(it.key());
    std::tm tm = {};
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
      slog::err << "Failed to parse timestamp: " << it.key() << slog::endl;
      ++it;
      continue;
    }
    auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    if (timestamp < oneWeekAgo) {
      slog::debug << "Erasing timestamp: "
                  << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << slog::endl;
      it = data.erase(it);
    } else {
      ++it;
    }
  }

  try {
    saveJsonData(data, dataFilePath);
  } catch (const std::exception &e) {
    slog::err << "Error saving data file: " << e.what() << slog::endl;
    throw;
  }

  slog::debug << "Successfully removed old data from: " << dataFilePath
              << slog::endl;
}
/**
 * @brief Creates an empty data file for storing gaze data.
 */
void VisionGuard::createEmptyDataFile() {
  nlohmann::json emptyData = nlohmann::json::object();
  saveJsonData(emptyData, dataFilePath);
  slog::debug << "Created new data file with empty JSON object: "
              << dataFilePath << slog::endl;
}

void VisionGuard::setCalibrationPoints(
    const ScreenCalibration &calibrationPoints) {
  calibration = calibrationPoints;
}

void VisionGuard::defaultCalibration(
    const cv::Size &imageSize = cv::Size(1920, 1080)) {
  this->setCalibrationPoints(this->getDefaultCalibrationPoints(imageSize));
}

void VisionGuard::customCalibration(
    const cv::Size &imageSize = cv::Size(1920, 1080)) {
  this->fourPointCalibration(imageSize);
}

void VisionGuard::fourPointCalibration(const cv::Size &imageSize) {
  cv::Mat calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
  ScreenCalibration calibration;

  // Define calibration points (corners)
  calibration.topLeft =
      cv::Point2f(0.1f * imageSize.width, 0.1f * imageSize.height);
  calibration.topRight =
      cv::Point2f(0.9f * imageSize.width, 0.1f * imageSize.height);
  calibration.bottomRight =
      cv::Point2f(0.9f * imageSize.width, 0.9f * imageSize.height);
  calibration.bottomLeft =
      cv::Point2f(0.1f * imageSize.width, 0.9f * imageSize.height);

  // Draw circles on calibration points
  const std::vector<cv::Point2f> points = {
      calibration.topLeft, calibration.topRight, calibration.bottomRight,
      calibration.bottomLeft};
  for (const auto &point : points) {
    calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
    cv::circle(calibrationImage, point, 10, cv::Scalar(0, 255, 0), -1);
    cv::imshow("Calibration", calibrationImage);
    cv::waitKey(1000); // Wait for 1 second
  }
  cv::destroyWindow("Calibration");

  // Log the calibration points
  slog::debug << calibration << slog::endl;

  // Set the calibration points
  setCalibrationPoints(calibration);
}

ScreenCalibration
VisionGuard::getDefaultCalibrationPoints(const cv::Size &imageSize) {
  ScreenCalibration calibration;

  calibration.topLeft =
      cv::Point2f(0.1f * imageSize.width, 0.1f * imageSize.height);
  calibration.topRight =
      cv::Point2f(0.9f * imageSize.width, 0.1f * imageSize.height);
  calibration.bottomRight =
      cv::Point2f(0.9f * imageSize.width, 0.9f * imageSize.height);
  calibration.bottomLeft =
      cv::Point2f(0.1f * imageSize.width, 0.9f * imageSize.height);

  return calibration;
}

double VisionGuard::getAccumulatedGazeTime() const {
  return accumulatedGazeTime;
}

/**
 * @brief Gets the available devices for running the models.
 * @return A vector of available devices.
 */
std::vector<std::string> VisionGuard::getAvailableDevices() {
  auto devices = core.get_available_devices();
  if (devices.size())
    devices.push_back("AUTO");
  return devices;
}

/**
 * @brief Gets the daily statistics of gaze time.
 * @return A map of daily gaze time statistics.
 */
std::map<std::string, double> VisionGuard::getDailyStats() {
  nlohmann::json data = readDataFile();
  std::map<std::string, double> dailyStats;

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&time);
  char buffer[11];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now_tm);
  std::string today(buffer);

  // Initialize 24 bins for each hour
  for (int i = 0; i < 24; ++i) {
    char hourBin[3];
    std::snprintf(hourBin, sizeof(hourBin), "%02d", i);
    dailyStats[hourBin] = 0;
  }

  for (auto &[key, value] : data.items()) {
    if (key.substr(0, 10) == today) {
      std::string hourBin = key.substr(11, 2);
      dailyStats[hourBin] += value["gaze_time"].get<double>();
    }
  }

  return dailyStats;
}

/**
 * @brief Gets the duration of time when the gaze was lost.
 * @return The gaze lost duration in seconds.
 */
double VisionGuard::getGazeLostDuration() const {
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       gazeLostTime)
      .count();
}

/**
 * @brief Gets the hourly timestamp for logging gaze data.
 * @return The hourly timestamp as a string.
 */
std::string VisionGuard::getHourlyTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&time);

  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:00:00", now_tm);
  return std::string(buffer);
}

/**
 * @brief Gets the weekly statistics of gaze time.
 * @return A map of weekly gaze time statistics.
 */
std::map<std::string, double> VisionGuard::getWeeklyStats() {
  nlohmann::json data = readDataFile();
  std::map<std::string, double> weeklyStats;

  auto now = std::chrono::system_clock::now();
  auto oneWeekAgo = now - std::chrono::hours(24 * 7);

  for (auto &[key, value] : data.items()) {
    std::istringstream ss(key);
    std::tm tm = {};
    ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    if (ss.fail()) {
      slog::err << "Failed to parse timestamp: " << key << slog::endl;
      continue;
    }
    auto timestamp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    if (timestamp >= oneWeekAgo) {
      std::string date = key.substr(2, 8);
      weeklyStats[date] += value["gaze_time"].get<double>();
    }
  }

  return weeklyStats;
}

bool VisionGuard::isDeviceAvailable(const std::string &device) {
  std::vector<std::string> availableDevices = getAvailableDevices();
  return std::find(availableDevices.begin(), availableDevices.end(), device) !=
         availableDevices.end();
}

void VisionGuard::logGazeData() {
  double gazeDuration = getAccumulatedGazeTime();
  nlohmann::json data = readDataFile();

  updateHourlyData(data, "gaze_time", gazeDuration);

  try {
    saveJsonData(data, dataFilePath);
  } catch (const std::exception &e) {
    slog::err << "Error saving data file: " << e.what() << slog::endl;
    throw;
  }
  slog::debug << "Hourly logs successfully updated: " << dataFilePath
              << slog::endl;
}

void VisionGuard::processFrame(cv::Mat &frame) {
  start_time = std::chrono::steady_clock::now();
  auto inferenceResults = faceDetector.detect(frame);
  for (auto &inferenceResult : inferenceResults) {
    for (auto estimator : estimators) {
      estimator->estimate(frame, inferenceResult);
    }
  }

  for (auto const &inferenceResult : inferenceResults) {
    resultsMarker.mark(frame, inferenceResult);
  }

  if (!inferenceResults.empty()) {
    updateGazeTime(inferenceResults[0].gazeVector, frame.size());
  }

  // Display accumulated gaze time and gaze lost duration on the frame
  cv::putText(frame, cv::format("Gaze Time: %.2f seconds", accumulatedGazeTime),
              cv::Point(10, frame.rows - 60), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(0, 255, 0), 1);
  std::chrono::duration<double> gazeLostDuration =
      std::chrono::steady_clock::now() - gazeLostTime;
  cv::putText(
      frame,
      cv::format("Gaze Lost Duration: %.2f seconds", gazeLostDuration.count()),
      cv::Point(10, frame.rows - 30), cv::FONT_HERSHEY_PLAIN, 1.0,
      cv::Scalar(0, 0, 255), 1);

  presenter.drawGraphs(frame);
  metrics.update(start_time, frame, {10, 22}, cv::FONT_HERSHEY_COMPLEX, 0.65);
}

nlohmann::json VisionGuard::readDataFile() {
  nlohmann::json data;

  try {
    std::ifstream inFile(dataFilePath);
    if (inFile.is_open()) {
      if (inFile.peek() == std::ifstream::traits_type::eof()) {
        inFile.close();
        createEmptyDataFile();
        data = nlohmann::json::object();
      } else {
        inFile >> data;
        inFile.close();
        slog::debug << "Data file read successfully: " << dataFilePath
                    << slog::endl;
      }
    } else {
      slog::err << "Unable to open file for reading: " << dataFilePath
                << slog::endl;
      createEmptyDataFile();
      data = nlohmann::json::object();
    }
  } catch (const std::exception &e) {
    slog::err << "Error reading data file: " << e.what() << slog::endl;
    throw;
  }

  return data;
}

void VisionGuard::resetGazeTime() {
  logGazeData();
  accumulatedGazeTime = 0;
}

void VisionGuard::saveJsonData(const nlohmann::json &data,
                               const std::string &filePath) {
  try {
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
      outFile << data.dump(4); // Write JSON object to file with indentation
      outFile.close();
      slog::debug << "Data file saved successfully: " << filePath << slog::endl;
    } else {
      slog::err << "Unable to open file for writing: " << filePath
                << slog::endl;
      throw std::runtime_error("File open error while saving data file");
    }
  } catch (const std::exception &e) {
    slog::err << "Error saving data file: " << e.what() << slog::endl;
    throw;
  }
}

void VisionGuard::setAccumulatedGazeTimeThreshold(
    const double accumulated_gaze_time_threshold) {
  this->accumulated_gaze_time_threshold = accumulated_gaze_time_threshold;
}

void VisionGuard::setGazeLostThreshold(const double gazeLostThreshold) {
  this->gazeLostThreshold = gazeLostThreshold;
}

void VisionGuard::toggle(int key) {
  toggleStates[key] = !toggleStates[key];
  resultsMarker.toggle(key);
  presenter.handleKey(key);
}

bool VisionGuard::isToggled(char toggleType) const {
  auto it = toggleStates.find(toggleType);
  if (it != toggleStates.end()) {
    return it->second;
  }
  return false;
}

bool VisionGuard::isPointInsidePolygon(const ScreenCalibration &calibration,
                                       const cv::Point2f &point) const {
  // Extract the four points from the ScreenCalibration struct
  std::vector<cv::Point2f> polygon = {calibration.topLeft, calibration.topRight,
                                      calibration.bottomRight,
                                      calibration.bottomLeft};

  int i, j, nvert = polygon.size();
  bool isInside = false;

  for (i = 0, j = nvert - 1; i < nvert; j = i++) {
    // Check if point is inside the polygon using the ray-casting algorithm
    if (((polygon[i].y > point.y) != (polygon[j].y > point.y)) &&
        (point.x < (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
                           (polygon[j].y - polygon[i].y) +
                       polygon[i].x)) {
      isInside = !isInside;
    }
  }

  return isInside;
}

void VisionGuard::updateGazeTime(const cv::Point3f &gazeVector,
                                 const cv::Size &imageSize) {

  auto now = std::chrono::steady_clock::now();
  cv::Point2f gazePoint(
      imageSize.width / 2 + gazeVector.x * imageSize.width / 2,
      imageSize.height / 2 - gazeVector.y * imageSize.height / 2);

  if (isPointInsidePolygon(calibration, gazePoint)) {
    if (!isGazingAtScreen) {
      isGazingAtScreen = true;
      lastCheckTime = now;
    } else {
      std::chrono::duration<double> elapsed = now - lastCheckTime;
      accumulatedGazeTime += elapsed.count();
      lastCheckTime = now;
    }
    gazeLostTime = now;
  } else {
    isGazingAtScreen = false;
    double gazeLostDuration = getGazeLostDuration();
    double accumulatedGazeTime = getAccumulatedGazeTime();
    if (gazeLostDuration > gazeLostThreshold && accumulatedGazeTime != 0) {
      slog::debug << "Gaze lost duration: " << gazeLostDuration
                  << " Gaze lost threshold: " << gazeLostThreshold
                  << slog::endl;
      resetGazeTime();
    }
  }
}

void VisionGuard::updateHourlyData(nlohmann::json &data, const std::string &key,
                                   double value) {
  std::string hour = getHourlyTimestamp();

  if (data.contains(hour)) {
    data[hour][key] = data[hour][key].get<double>() + value;
  } else {
    data[hour][key] = value;
  }
}