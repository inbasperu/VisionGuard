#include "eye_gaze_time_tracker.hpp"
#include <chrono>
#include <opencv2/core.hpp>
#include <utils/slog.hpp>

EyeGazeTimeTracker::EyeGazeTimeTracker()
    : accumulatedGazeTime(0), accumulatedGazeTimeThreshold(20),
      gazeLostThreshold(10), isGazingAtScreen(false),
      gazeLostTime(std::chrono::steady_clock::now()),
      lastCheckTime(std::chrono::steady_clock::now()) {}

// Getters
double EyeGazeTimeTracker::getAccumulatedGazeTime() const {
  return accumulatedGazeTime;
}

bool EyeGazeTimeTracker::checkGazeTimeExceeded() const {
  return accumulatedGazeTime >= accumulatedGazeTimeThreshold;
}

// Setters
void EyeGazeTimeTracker::setAccumulatedGazeTimeThreshold(
    const double threshold) {
  accumulatedGazeTimeThreshold = threshold;
}

void EyeGazeTimeTracker::setGazeLostThreshold(const double threshold) {
  gazeLostThreshold = threshold;
}

// Other member functions
void EyeGazeTimeTracker::resetGazeTime() {
  //! The data is not being saved before being reset.
  accumulatedGazeTime = 0;
}

void EyeGazeTimeTracker::updateGazeTime(const cv::Point3f &gazeVector,
                                        cv::Mat &frame) {

  const cv::Size imageSize = frame.size();
  auto now = std::chrono::steady_clock::now();
  cv::Point2f gazePoint(
      imageSize.width / 2 + gazeVector.x * imageSize.width / 2,
      imageSize.height / 2 - gazeVector.y * imageSize.height / 2);

  if (isGazingAtScreen) {
    std::chrono::duration<double> elapsed = now - lastCheckTime;
    accumulatedGazeTime += elapsed.count();
    lastCheckTime = now;
    gazeLostTime = now;
  } else {
    double gazeLostDuration =
        std::chrono::duration<double>(now - gazeLostTime).count();
    if (gazeLostDuration > gazeLostThreshold) {
      resetGazeTime();
    }
  }
  updateGazeTime(frame);
}

void EyeGazeTimeTracker::updateGazeTime(cv::Mat &frame) {
  // Display accumulated gaze time and gaze lost duration on the frame
  cv::putText(frame,
              cv::format("Gaze Time: %.2f seconds", getAccumulatedGazeTime()),
              cv::Point(10, frame.rows - 60), cv::FONT_HERSHEY_PLAIN, 1.0,
              cv::Scalar(0, 255, 0), 1);
  cv::putText(
      frame,
      cv::format("Gaze Lost Duration: %.2f seconds", getGazeLostDuration()),
      cv::Point(10, frame.rows - 30), cv::FONT_HERSHEY_PLAIN, 1.0,
      cv::Scalar(0, 0, 255), 1);
}