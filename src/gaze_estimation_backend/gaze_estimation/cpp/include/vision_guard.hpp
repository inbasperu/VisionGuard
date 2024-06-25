#ifndef VISIONGUARD_H
#define VISIONGUARD_H

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "openvino/openvino.hpp"

#include <gflags/gflags.h>
#include <monitors/presenter.h>
#include <nlohmann/json.hpp>
#include <utils/args_helper.hpp>
#include <utils/images_capture.h>
#include <utils/ocv_common.hpp>
#include <utils/performance_metrics.hpp>
#include <utils/slog.hpp>

#include "base_estimator.hpp"
#include "eye_state_estimator.hpp"
#include "face_detector.hpp"
#include "face_inference_results.hpp"
#include "gaze_estimator.hpp"
#include "head_pose_estimator.hpp"
#include "landmarks_estimator.hpp"
#include "results_marker.hpp"
#include "utils.hpp"

struct ScreenCalibration {
  std::vector<cv::Point2f> points; // Calibration points on the screen
  bool isCalibrated = false;
};

class VisionGuard {
public:
  VisionGuard(const std::string &gaze_model, const std::string &face_model,
              const std::string &head_pose_model,
              const std::string &landmarks_model,
              const std::string &eye_state_model, const std::string &device);
  ~VisionGuard();

  std::vector<std::string> getAvailableDevices();
  void processFrame(cv::Mat &frame);
  void showCalibrationWindow(const cv::Size &imageSize);
  void defaultCalibration(const cv::Size &imageSize);
  void calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints);
  double getAccumulatedGazeTime() const;
  double getGazeLostDuration() const;
  void toggle(int key);
  bool isDeviceAvailable(const std::string &device);
  bool checkGazeTimeExceeded() const;
  void resetGazeTime();
  void
  setAccumulatedGazeTimeThreshold(const double accumulated_gaze_time_threshold);
  void setGazeLostThreshold(const double gazeLostThreshold);
  std::map<std::string, double> getDailyStats();
  std::map<std::string, double> getWeeklyStats();

private:
  void updateGazeTime(const cv::Point3f &gazeVector, const cv::Size &imageSize);
  bool isPointInsidePolygon(const std::vector<cv::Point2f> &polygon,
                            const cv::Point2f &point) const;
  std::vector<cv::Point2f>
  getDefaultCalibrationPoints(const cv::Size &imageSize,
                              const int numPoints = 5);
  ScreenCalibration calibration;
  std::chrono::steady_clock::time_point lastCheckTime;
  std::chrono::steady_clock::time_point gazeLostTime;
  double accumulatedGazeTime = 0; // In seconds
  bool isGazingAtScreen = false;
  double accumulated_gaze_time_threshold = 20;
  double gazeLostThreshold = 10;

  void logGazeData();
  void cleanOldData();

  void updateHourlyData(nlohmann::json &data, const std::string &key,
                        double value);
  std::string getHourlyTimestamp() const;

  nlohmann::json readDataFile();
  void saveJsonData(const nlohmann::json &data, const std::string &filePath);
  void createEmptyDataFile();
  std::string dataFilePath = "screen_time_stats.json";

  ov::Core core;
  gaze_estimation::FaceDetector faceDetector;
  gaze_estimation::HeadPoseEstimator headPoseEstimator;
  gaze_estimation::LandmarksEstimator landmarksEstimator;
  gaze_estimation::EyeStateEstimator eyeStateEstimator;
  gaze_estimation::GazeEstimator gazeEstimator;
  gaze_estimation::ResultsMarker resultsMarker;
  std::vector<gaze_estimation::BaseEstimator *> estimators;
};

#endif // VISIONGUARD_H
