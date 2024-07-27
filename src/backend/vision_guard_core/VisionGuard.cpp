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
  // Log gaze data and clean old data before destroying the object
  screenTimeMetricLogger.logGazeData(
      eyeGazeTimeTracker.getAccumulatedGazeTime());
  screenTimeMetricLogger.cleanOldData();
}

void VisionGuard::processFrame(cv::Mat &frame) {
  gazeDetectionEngine.processFrame(frame);
  if (vectorCalibration.isCalibrated()) {
    auto gazeVector = gazeDetectionEngine.getGazeVector();
    eyeGazeTimeTracker.updateGazeTime(gazeVector, frame.size());
    if (eyeGazeTimeTracker.checkGazeTimeExceeded()) {
      breakNotificationSystem.notifyBreak();
    }
  }

  // Display accumulated gaze time and gaze lost duration on the frame
  cv::putText(frame,
              cv::format("Gaze Time: %.2f seconds",
                         eyeGazeTimeTracker.getAccumulatedGazeTime()),
              cv::Point(10, frame.rows - 60), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(0, 255, 0), 1);
  cv::putText(frame,
              cv::format("Gaze Lost Duration: %.2f seconds",
                         eyeGazeTimeTracker.getGazeLostDuration()),
              cv::Point(10, frame.rows - 30), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(0, 0, 255), 1);

  performanceTracker.update(frame);
}

void VisionGuard::toggle(int key) {
  gazeDetectionEngine.toggle(key);
  performanceTracker.toggle(key);
}
