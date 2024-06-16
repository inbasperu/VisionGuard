#include "vision_guard.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <utils/slog.hpp>

VisionGuard::VisionGuard(const std::string &gaze_model,
                         const std::string &face_model,
                         const std::string &head_pose_model,
                         const std::string &landmarks_model,
                         const std::string &eye_state_model,
                         const std::string &device)
    : faceDetector(core, face_model, device, 0.5, false),
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
    gazeLostTime = now; // Reset gaze lost timer
  } else {
    isGazingAtScreen = false;
    std::chrono::duration<double> gazeLostDuration = now - gazeLostTime;
    // Reset timer if gaze lost for more than gazeLostThreshold
    if (gazeLostDuration.count() > gazeLostThreshold) {
      accumulatedGazeTime = 0;
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
void VisionGuard::resetGazeTime() { accumulatedGazeTime = 0; }

void VisionGuard::setAccumulatedGazeTimeThreshold(
    const double accumulated_gaze_time_threshold) {
  this->accumulated_gaze_time_threshold = accumulated_gaze_time_threshold;
}

void VisionGuard::setGazeLostThreshold(const double gazeLostThreshold) {
  this->gazeLostThreshold = gazeLostThreshold;
}