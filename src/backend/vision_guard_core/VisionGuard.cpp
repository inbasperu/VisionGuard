#include "VisionGuard.hpp"
#include <utils/slog.hpp>

VisionGuard::VisionGuard(const std::string &gaze_model,
                         const std::string &face_model,
                         const std::string &head_pose_model,
                         const std::string &landmarks_model,
                         const std::string &eye_state_model,
                         const std::string &device, const cv::Size &screenSize,
                         const std::string &screenTimeStatsFilePath)
    : gazeDetectionEngine(face_model, head_pose_model, landmarks_model,
                          eye_state_model, device, core),
      vectorCalibration(screenSize), eyeGazeTimeTracker(),
      breakNotificationSystem(), performanceTracker(),
      screenTimeMetricLogger(screenTimeStatsFilePath) {
  slog::info << ov::get_openvino_version() << slog::endl;
  slog::info << "Available Devices:";
  slog::info << gazeDetectionEngine.getAvailableDevices() << slog::endl;
}

VisionGuard::~VisionGuard() {
  slog::debug << "Destroying VisionGuard Object" << slog::endl;
  slog::info << "Metrics report:" << slog::endl;
  performanceTracker.getMetrics().logTotal();
  screenTimeMetricLogger.logGazeData(
      eyeGazeTimeTracker.getAccumulatedGazeTime());
  screenTimeMetricLogger.cleanOldData();
}

void VisionGuard::processFrame(cv::Mat &frame) {
  gazeDetectionEngine.processFrame(frame);
  if (vectorCalibration.isCalibrated()) {
    auto gazeVector = gazeDetectionEngine.getGazeVector();
    eyeGazeTimeTracker.updateGazeTime(gazeVector, frame);
    if (eyeGazeTimeTracker.checkGazeTimeExceeded()) {
      breakNotificationSystem.notifyBreak();
    }
  }
  performanceTracker.update(frame);
}

void VisionGuard::toggle(int key) {
  gazeDetectionEngine.toggle(key);
  performanceTracker.toggle(key);
}
