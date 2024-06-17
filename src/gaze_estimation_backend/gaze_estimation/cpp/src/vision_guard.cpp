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
    : lastCheckTime(std::chrono::steady_clock::now()),
      gazeLostTime(std::chrono::steady_clock::now()),
      faceDetector(core, face_model, device, 0.5, false),
      headPoseEstimator(core, head_pose_model, device),
      landmarksEstimator(core, landmarks_model, device),
      eyeStateEstimator(core, eye_state_model, device),
      gazeEstimator(core, gaze_model, device),
      resultsMarker(false, false, false, true, true) {
  // Load OpenVINO runtime
  slog::info << ov::get_openvino_version() << slog::endl;
  estimators = {&headPoseEstimator, &landmarksEstimator, &eyeStateEstimator,
                &gazeEstimator};
  slog::info << "Avilable Devices:";
  slog::info << getAvailableDevices() << slog::endl;
}

VisionGuard::~VisionGuard() {
  slog::debug << "Destroying VisionGuard Object" << slog::endl;
  // Log gaze data and clean old data before destroying the object
  logGazeData();
  cleanOldData();
}
void VisionGuard::toggle(int key) { resultsMarker.toggle(key); }
std::vector<std::string> VisionGuard::getAvailableDevices() {
  return core.get_available_devices();
}

bool VisionGuard::isDeviceAvailable(const std::string &device) {
  std::vector<std::string> availableDevices = getAvailableDevices();
  return std::find(availableDevices.begin(), availableDevices.end(), device) !=
         availableDevices.end();
}

void VisionGuard::calibrateScreen(
    const std::vector<cv::Point2f> &calibrationPoints) {
  calibration.points = calibrationPoints;
  calibration.isCalibrated = true;
}

void VisionGuard::defaultCalibration(const cv::Size &imageSize) {
  this->calibrateScreen(this->getDefaultCalibrationPoints(imageSize));
}

std::vector<cv::Point2f>
VisionGuard::getDefaultCalibrationPoints(const cv::Size &imageSize,
                                         const int numPoints) {
  std::vector<cv::Point2f> calibrationPoints;
  // Define calibration points (corners and center)
  calibrationPoints.push_back(
      cv::Point2f(0.1 * imageSize.width, 0.1 * imageSize.height)); // Top-left
  calibrationPoints.push_back(
      cv::Point2f(0.9 * imageSize.width, 0.1 * imageSize.height)); // Top-right
  calibrationPoints.push_back(
      cv::Point2f(0.5 * imageSize.width, 0.5 * imageSize.height)); // Center
  calibrationPoints.push_back(cv::Point2f(
      0.1 * imageSize.width, 0.9 * imageSize.height)); // Bottom-left
  calibrationPoints.push_back(cv::Point2f(
      0.9 * imageSize.width, 0.9 * imageSize.height)); // Bottom-right
  return calibrationPoints;
}

void VisionGuard::showCalibrationWindow(const cv::Size &imageSize) {
  const int numPoints = 5;
  cv::Mat calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
  std::vector<cv::Point2f> calibrationPoints;

  // Define calibration points (corners and center)
  calibrationPoints.push_back(
      cv::Point2f(0.1 * imageSize.width, 0.1 * imageSize.height)); // Top-left
  calibrationPoints.push_back(
      cv::Point2f(0.9 * imageSize.width, 0.1 * imageSize.height)); // Top-right
  calibrationPoints.push_back(
      cv::Point2f(0.5 * imageSize.width, 0.5 * imageSize.height)); // Center
  calibrationPoints.push_back(cv::Point2f(
      0.1 * imageSize.width, 0.9 * imageSize.height)); // Bottom-left
  calibrationPoints.push_back(cv::Point2f(
      0.9 * imageSize.width, 0.9 * imageSize.height)); // Bottom-right

  for (int i = 0; i < numPoints; ++i) {
    calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
    cv::circle(calibrationImage, calibrationPoints[i], 10,
               cv::Scalar(0, 255, 0), -1);
    cv::imshow("Calibration", calibrationImage);
    cv::waitKey(1000); // Wait for 1 second
  }
  cv::destroyWindow("Calibration");
  calibrateScreen(calibrationPoints);
}

bool VisionGuard::isPointInsidePolygon(const std::vector<cv::Point2f> &polygon,
                                       const cv::Point2f &point) const {
  int i, j, nvert = polygon.size();
  bool c = false;
  for (i = 0, j = nvert - 1; i < nvert; j = i++) {
    if (((polygon[i].y >= point.y) != (polygon[j].y >= point.y)) &&
        (point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
                            (polygon[j].y - polygon[i].y) +
                        polygon[i].x)) {
      c = !c;
    }
  }
  return c;
}

void VisionGuard::updateGazeTime(const cv::Point3f &gazeVector,
                                 const cv::Size &imageSize) {
  if (!calibration.isCalibrated)
    return;

  auto now = std::chrono::steady_clock::now();
  cv::Point2f gazePoint(
      imageSize.width / 2 + gazeVector.x * imageSize.width / 2,
      imageSize.height / 2 - gazeVector.y * imageSize.height /
                                 2); // Convert gaze vector to 2D point

  if (isPointInsidePolygon(calibration.points, gazePoint)) {
    if (!isGazingAtScreen) {
      isGazingAtScreen = true;
      lastCheckTime = now;
    } else {
      std::chrono::duration<double> elapsed = now - lastCheckTime;
      accumulatedGazeTime += elapsed.count();
      lastCheckTime = now;
    }
    // Reset gaze lost timer
    gazeLostTime = now;
  } else {
    isGazingAtScreen = false;
    double gazeLostDuration = getGazeLostDuration();
    double accumulatedGazeTime = getAccumulatedGazeTime();
    // Reset timer if gaze lost for more than gazeLostThreshold and accumulated
    // gaze time is not 0
    if (gazeLostDuration > gazeLostThreshold && accumulatedGazeTime != 0) {
      slog::debug << "Gaze lost duration: " << gazeLostDuration
                  << " Gaze lost threshold: " << gazeLostThreshold
                  << slog::endl;
      resetGazeTime();
    }
  }
}

void VisionGuard::processFrame(cv::Mat &frame) {
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
}

double VisionGuard::getAccumulatedGazeTime() const {
  return accumulatedGazeTime;
}

double VisionGuard::getGazeLostDuration() const {
  return std::chrono::duration<double>(std::chrono::steady_clock::now() -
                                       gazeLostTime)
      .count();
}

bool VisionGuard::checkGazeTimeExceeded() const {
  return accumulatedGazeTime >= accumulated_gaze_time_threshold;
}
void VisionGuard::resetGazeTime() {

  // Log gaze data before resetting the timer
  logGazeData();
  accumulatedGazeTime = 0;
}

void VisionGuard::setAccumulatedGazeTimeThreshold(
    const double accumulated_gaze_time_threshold) {
  this->accumulated_gaze_time_threshold = accumulated_gaze_time_threshold;
}

void VisionGuard::setGazeLostThreshold(const double gazeLostThreshold) {
  this->gazeLostThreshold = gazeLostThreshold;
}

void VisionGuard::logGazeData() {
  double gazeDuration = getAccumulatedGazeTime();
  nlohmann::json data;

  try {
    std::fstream file(dataFilePath,
                      std::ios::in | std::ios::out | std::ios::binary);
    lockFile(file);

    if (file.is_open()) {
      file.seekg(0, std::ios::end);
      size_t size = file.tellg();
      if (size > 0) {
        file.seekg(0, std::ios::beg);
        file >> data;
      } else {
        data = nlohmann::json::object();
      }
      file.close();
    } else {
      slog::err << "Unable to open file for reading: " << dataFilePath
                << slog::endl;
      throw std::runtime_error("File open error");
    }
  } catch (const std::exception &e) {
    slog::err << "Error reading data file: " << e.what() << slog::endl;
    throw;
  }

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

void VisionGuard::cleanOldData() {
  auto now = std::chrono::system_clock::now();
  auto oneWeekAgo = now - std::chrono::hours(24 * 7);
  nlohmann::json data;

  try {
    std::fstream file(dataFilePath,
                      std::ios::in | std::ios::out | std::ios::binary);
    lockFile(file);

    if (file.is_open()) {
      file.seekg(0, std::ios::end);
      size_t size = file.tellg();
      if (size > 0) {
        file.seekg(0, std::ios::beg);
        file >> data;
      } else {
        data = nlohmann::json::object();
      }
      file.close();
    } else {
      slog::err << "Unable to open file for reading: " << dataFilePath
                << slog::endl;
      throw std::runtime_error("File open error");
    }
  } catch (const std::exception &e) {
    slog::err << "Error reading data file: " << e.what() << slog::endl;
    throw;
  }

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
      // Use std::put_time to format the time
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

void VisionGuard::lockFile(std::fstream &file) {
#ifdef _WIN32
  HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
  OVERLAPPED ov = {0};
  if (LockFileEx(hFile, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &ov) ==
      0) {
    throw std::runtime_error("Failed to lock file");
  }
#else
  int fd = fileno(reinterpret_cast<FILE *>(file.rdbuf()));
  if (flock(fd, LOCK_EX) != 0) {
    throw std::runtime_error("Failed to lock file");
  }
#endif
}

void VisionGuard::unlockFile(std::fstream &file) {
#ifdef _WIN32
  HANDLE hFile = (HANDLE)_get_osfhandle(_fileno(file));
  OVERLAPPED ov = {0};
  if (UnlockFileEx(hFile, 0, MAXDWORD, MAXDWORD, &ov) == 0) {
    throw std::runtime_error("Failed to unlock file");
  }
#else
  int fd = fileno(reinterpret_cast<FILE *>(file.rdbuf()));
  if (flock(fd, LOCK_UN) != 0) {
    throw std::runtime_error("Failed to unlock file");
  }
#endif
}

std::map<std::string, double> VisionGuard::getDailyStats() {
  nlohmann::json data;
  std::map<std::string, double> dailyStats;

  try {
    std::ifstream inFile(dataFilePath);
    if (inFile.is_open()) {
      inFile >> data;
      inFile.close();
    } else {
      slog::err << "Unable to open file for reading: " << dataFilePath
                << slog::endl;
      throw std::runtime_error("File open error");
    }
  } catch (const std::exception &e) {
    slog::err << "Error reading data file: " << e.what() << slog::endl;
    throw;
  }

  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&time);
  char buffer[11];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", now_tm);
  std::string today(buffer);

  // Initialize 24 bins for each hour
  for (int i = 0; i < 24; ++i) {
    char hourBin[6];
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

std::map<std::string, double> VisionGuard::getWeeklyStats() {
  nlohmann::json data;
  std::map<std::string, double> weeklyStats;

  try {
    std::ifstream inFile(dataFilePath);
    if (inFile.is_open()) {
      inFile >> data;
      inFile.close();
    } else {
      slog::err << "Unable to open file for reading: " << dataFilePath
                << slog::endl;
      throw std::runtime_error("File open error");
    }
  } catch (const std::exception &e) {
    slog::err << "Error reading data file: " << e.what() << slog::endl;
    throw;
  }

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

std::string VisionGuard::getHourlyTimestamp() const {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  std::tm *now_tm = std::localtime(&time);

  char buffer[20];
  std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:00:00", now_tm);
  return std::string(buffer);
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

void VisionGuard::saveJsonData(const nlohmann::json &data,
                               const std::string &filePath) {
  std::ofstream outFile(filePath, std::ios::trunc);
  if (!outFile.is_open()) {
    slog::err << "Unable to open file for writing: " << filePath << slog::endl;
    throw std::runtime_error("File open error");
  }

  outFile << data.dump(4);
  outFile.close();
}