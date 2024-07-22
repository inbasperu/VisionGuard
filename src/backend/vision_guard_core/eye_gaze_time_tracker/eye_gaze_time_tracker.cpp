#include "eye_gaze_time_tracker.hpp"
#include <opencv2/core.hpp>
#include <chrono>
#include <utils/slog.hpp>

EyeGazeTimeTracker::EyeGazeTimeTracker()
    : accumulatedGazeTime(0), accumulatedGazeTimeThreshold(20), gazeLostThreshold(10),
      isGazingAtScreen(false), gazeLostTime(std::chrono::steady_clock::now()),
      lastCheckTime(std::chrono::steady_clock::now()) {}

void EyeGazeTimeTracker::updateGazeTime(const cv::Point3f &gazeVector, const cv::Size &imageSize) {
    auto now = std::chrono::steady_clock::now();
    cv::Point2f gazePoint(imageSize.width / 2 + gazeVector.x * imageSize.width / 2,
                          imageSize.height / 2 - gazeVector.y * imageSize.height / 2);

    if (isGazingAtScreen) {
        std::chrono::duration<double> elapsed = now - lastCheckTime;
        accumulatedGazeTime += elapsed.count();
        lastCheckTime = now;
        gazeLostTime = now;
    } else {
        double gazeLostDuration = std::chrono::duration<double>(now - gazeLostTime).count();
        if (gazeLostDuration > gazeLostThreshold) {
            resetGazeTime();
        }
    }
}

void EyeGazeTimeTracker::resetGazeTime() {
    accumulatedGazeTime = 0;
}

double EyeGazeTimeTracker::getAccumulatedGazeTime() const {
    return accumulatedGazeTime;
}

bool EyeGazeTimeTracker::checkGazeTimeExceeded() const {
    return accumulatedGazeTime >= accumulatedGazeTimeThreshold;
}

void EyeGazeTimeTracker::setAccumulatedGazeTimeThreshold(const double threshold) {
    accumulatedGazeTimeThreshold = threshold;
}

void EyeGazeTimeTracker::setGazeLostThreshold(const double threshold) {
    gazeLostThreshold = threshold;
}
