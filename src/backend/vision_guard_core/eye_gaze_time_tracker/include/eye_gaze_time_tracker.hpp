#ifndef EYE_GAZE_TIME_TRACKER_HPP
#define EYE_GAZE_TIME_TRACKER_HPP

#include <chrono>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class EyeGazeTimeTracker {
public:
  EyeGazeTimeTracker();

  // Getters
  double getAccumulatedGazeTime() const;
  bool checkGazeTimeExceeded() const;

  // Setters
  void setAccumulatedGazeTimeThreshold(const double threshold);
  void setGazeLostThreshold(const double threshold);

  // Other member functions
  void resetGazeTime();
  void updateGazeTime(const cv::Point3f &gazeVector, cv::Mat &frame);

private:
  void updateGazeTime(cv::Mat &frame);
  // Data members
  // TODO: change the names of `accumulatedGazeTime` => `screenGazeDuration`
  // TODO: change the names of `accumulatedGazeTimeThreshold` =>
  // screenGazeDurationThreshold
  // TODO: change the names of `gazeLostThreshold` =>
  // `screenBreakDurationThreshold`
  double accumulatedGazeTime;
  double accumulatedGazeTimeThreshold;
  double gazeLostThreshold;
  bool isGazingAtScreen;
  std::chrono::steady_clock::time_point gazeLostTime;
  std::chrono::steady_clock::time_point lastCheckTime;
};

#endif // EYE_GAZE_TIME_TRACKER_HPP
