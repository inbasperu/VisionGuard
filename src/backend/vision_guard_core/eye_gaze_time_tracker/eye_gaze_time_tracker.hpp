#ifndef EYE_GAZE_TIME_TRACKER_HPP
#define EYE_GAZE_TIME_TRACKER_HPP

#include <chrono>
#include <opencv2/core.hpp>

class EyeGazeTimeTracker {
public:
    EyeGazeTimeTracker();
    void updateGazeTime(const cv::Point3f &gazeVector, const cv::Size &imageSize);
    void resetGazeTime();
    double getAccumulatedGazeTime() const;
    bool checkGazeTimeExceeded() const;

    void setAccumulatedGazeTimeThreshold(const double threshold);
    void setGazeLostThreshold(const double threshold);

private:
    double accumulatedGazeTime;
    double accumulatedGazeTimeThreshold;
    double gazeLostThreshold;
    bool isGazingAtScreen;
    std::chrono::steady_clock::time_point gazeLostTime;
    std::chrono::steady_clock::time_point lastCheckTime;
};

#endif // EYE_GAZE_TIME_TRACKER_HPP
