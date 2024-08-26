#include "vision_guard.hpp"
#include <cmath>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <utils/slog.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <fcntl.h>
#include <sys/file.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <CoreGraphics/CoreGraphics.h>
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
      presenter("", 300, cv::Size(200, 40)) {
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

  dataFilePath = getDataFilePath();

  // Initialize calibration with default values
  calibration = getDefaultCalibrationPoints(cv::Size(1920, 1080));
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

bool VisionGuard::checkGazeTimeExceeded() const {
  return accumulatedGazeTime >= accumulated_gaze_time_threshold;
}

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

void VisionGuard::createEmptyDataFile() {
  nlohmann::json emptyData = nlohmann::json::object();
  saveJsonData(emptyData, dataFilePath);
  slog::debug << "Created new data file with empty JSON object: "
              << dataFilePath << slog::endl;
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

void VisionGuard::setCalibrationPoints(
    const ScreenCalibration &calibrationPoints) {
  calibration = calibrationPoints;
}

void VisionGuard::setCalibrationPoints(const cv::Point2f &topLeft,
                                       const cv::Point2f &topRight,
                                       const cv::Point2f &bottomRight,
                                       const cv::Point2f &bottomLeft) {
  calibration = ScreenCalibration({topLeft, topRight, bottomRight, bottomLeft});
  slog::debug << "New Calibration Points set to: "
              << "Top Left (" << topLeft.x << ", " << topLeft.y << "), "
              << "Top Right (" << topRight.x << ", " << topRight.y << "), "
              << "Bottom Right (" << bottomRight.x << ", " << bottomRight.y
              << "), "
              << "Bottom Left (" << bottomLeft.x << ", " << bottomLeft.y << ")"
              << slog::endl;
}

void VisionGuard::defaultCalibration(
    const cv::Size &imageSize = cv::Size(1920, 1080)) {
  this->setCalibrationPoints(this->getDefaultCalibrationPoints(imageSize));
}

ScreenCalibration
VisionGuard::getDefaultCalibrationPoints(const cv::Size &imageSize) {
  slog::debug << "Performing default calibration for screen size " << imageSize
              << slog::endl;
  ScreenCalibration calibration;

  calibration.topLeft =
      cv::Point2f(0.1f * imageSize.width, 0.1f * imageSize.height);
  calibration.topRight =
      cv::Point2f(0.9f * imageSize.width, 0.1f * imageSize.height);
  calibration.bottomRight =
      cv::Point2f(0.9f * imageSize.width, 0.9f * imageSize.height);
  calibration.bottomLeft =
      cv::Point2f(0.1f * imageSize.width, 0.9f * imageSize.height);

  // Log the calibration points
  slog::debug << calibration << slog::endl;

  return calibration;
}

double VisionGuard::getAccumulatedGazeTime() const {
  return accumulatedGazeTime;
}

std::vector<std::string> VisionGuard::getAvailableDevices() {
  auto devices = core.get_available_devices();
  if (devices.size())
    devices.push_back("AUTO");
  return devices;
}

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

double VisionGuard::getGazeLostDuration() const {
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       gazeLostTime)
      .count();
}

std::string VisionGuard::getHourlyTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&time);

  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:00:00", now_tm);
  return std::string(buffer);
}

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

void VisionGuard::processFrame(cv::Mat &frame) {
  start_time = std::chrono::steady_clock::now();
  auto inferenceResults = faceDetector.detect(frame);
  for (auto &inferenceResult : inferenceResults) {
    for (auto estimator : estimators) {
      // &headPoseEstimator
      // &landmarksEstimator
      // &eyeStateEstimator
      // &gazeEstimator
      estimator->estimate(frame, inferenceResult);
    }
  }

  for (auto const &inferenceResult : inferenceResults) {
    resultsMarker.mark(frame, inferenceResult);
    updateGazeTime(inferenceResult, frame.size());
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
  metrics.update(start_time, frame, {12, 35}, cv::FONT_HERSHEY_COMPLEX, 0.65);
}

void VisionGuard::resetGazeTime() {
  logGazeData();
  accumulatedGazeTime = 0;
}

void VisionGuard::setAccumulatedGazeTimeThreshold(
    const double accumulated_gaze_time_threshold) {
  // accumulated_gaze_time_threshold is in min
  this->accumulated_gaze_time_threshold = accumulated_gaze_time_threshold * 60;
}

void VisionGuard::setGazeLostThreshold(const double gazeLostThreshold) {
  this->gazeLostThreshold = gazeLostThreshold;
}

void VisionGuard::toggle(int key) {
  if (toggleStates.find(key) != toggleStates.end()) {
    toggleStates[key] = !toggleStates[key];
    if (key == TOGGLE_RESOURCE_GRAPH)
      presenter.handleKey(key);
    else
      resultsMarker.toggle(key);
  } else {
    // Handle invalid key, maybe log a warning
    slog::warn << "Attempted to toggle invalid key: " << key << slog::endl;
  }
}

bool VisionGuard::isToggled(char toggleType) const {
  auto it = toggleStates.find(toggleType);
  if (it != toggleStates.end()) {
    return it->second;
  }
  return false;
}

cv::Point2f VisionGuard::convertGazeVectorToPointRel(
    const cv::Point3f &gazeVector, const cv::Size &screenSize,
    float cameraHeight, float screenWidth, float screenHeight) {
  // Normalize the gaze vector
  cv::Point3f normalizedGaze = gazeVector / cv::norm(gazeVector);

  // Calculate the intersection point with the screen plane
  // Assume the camera is at (0, cameraHeight, 0) and the screen center is at
  // (0, 0, screenHeight/2)
  float t = (screenHeight / 2) / normalizedGaze.z;
  float x = normalizedGaze.x * t;
  float y = normalizedGaze.y * t + cameraHeight;

  // Convert to normalized coordinates (-1 to 1)
  float normalizedX = x / (screenWidth / 2);
  float normalizedY = (y - screenHeight / 2) / (screenHeight / 2);

  // Convert to pixel coordinates
  float pixelX = (normalizedX + 1) * screenSize.width / 2;
  float pixelY = (1 - normalizedY) * screenSize.height / 2; // Flip Y-axis

  // // Clamp the coordinates to the screen boundaries
  // pixelX = std::max(0.0f,
  //                   std::min(pixelX, static_cast<float>(screenSize.width -
  //                   1)));
  // pixelY = std::max(
  //     0.0f, std::min(pixelY, static_cast<float>(screenSize.height - 1)));

  slog::debug << "Gaze Vector: (" << gazeVector.x << ", " << gazeVector.y
              << ", " << gazeVector.z << ")" << slog::endl;
  slog::debug << "Converted Point: (" << pixelX << ", " << pixelY << ")"
              << slog::endl;

  return cv::Point2f(pixelX, pixelY);
}

cv::Point2f VisionGuard::captureGazePoint(cv::Mat &frame) {
  if (frame.empty()) {
    throw std::runtime_error("Input frame is empty");
  }

  auto inferenceResults = faceDetector.detect(frame);
  if (inferenceResults.empty()) {
    throw std::runtime_error("No face detected in the frame");
  }

  for (auto &inferenceResult : inferenceResults) {
    for (auto estimator : estimators) {
      estimator->estimate(frame, inferenceResult);
    }
  }

  return convertGazeVectorToPointRel(inferenceResults.back().gazeVector,
                                     frame.size());
}

bool VisionGuard::isGazeInScreen(const ScreenCalibration &calibration,
                                 const cv::Point3f &gazeVector,
                                 const cv::Size &imageSize) {
  // Extract the four points from the ScreenCalibration struct
  std::vector<cv::Point2f> polygon = {calibration.topLeft, calibration.topRight,
                                      calibration.bottomRight,
                                      calibration.bottomLeft};
  auto point = convertGazeVectorToPointRel(gazeVector, imageSize);

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

  slog::debug << "Gaze is " << (isInside ? "inside" : "outside")
              << " the screen" << slog::endl;

  return isInside;
}

void VisionGuard::updateGazeTime(
    const gaze_estimation::FaceInferenceResults &faceInferenceResults,
    const cv::Size &imageSize) {
  auto now = std::chrono::steady_clock::now();

  bool isEyeOpen =
      faceInferenceResults.leftEyeState && faceInferenceResults.rightEyeState;

  bool isGazeOnScreen =
      isEyeOpen &&
      isGazeInScreen(calibration, faceInferenceResults.gazeVector, imageSize);

  if (isGazeOnScreen) {
    if (!isGazingAtScreen) {
      // User started gazing at the screen
      isGazingAtScreen = true;
      lastCheckTime = now;
      // slog::debug << "Gaze started at the screen." << slog::endl;
    } else {
      // User continues gazing at the screen
      std::chrono::duration<double> elapsed = now - lastCheckTime;
      accumulatedGazeTime += elapsed.count();
      lastCheckTime = now;
      // slog::debug << "Accumulated Gaze Time: " << accumulatedGazeTime
      //             << " seconds" << slog::endl;
    }
    gazeLostTime = now;
  } else {
    // User is not gazing at the screen
    isGazingAtScreen = false;
    std::chrono::duration<double> elapsed = now - gazeLostTime;
    double gazeLostDuration = elapsed.count();

    // slog::debug << "Gaze lost duration: " << gazeLostDuration << " seconds"
    //             << slog::endl;

    if (gazeLostDuration > gazeLostThreshold) {
      slog::debug
          << "Gaze lost duration exceeded threshold. Resetting gaze time."
          << slog::endl;
      gazeLostTime = now;
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