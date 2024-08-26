#pragma once

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

#include <QDir>
#include <QStandardPaths>
#include <QString>

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
#include "constants.hpp"
#include "eye_state_estimator.hpp"
#include "face_detector.hpp"
#include "face_inference_results.hpp"
#include "gaze_estimator.hpp"
#include "head_pose_estimator.hpp"
#include "landmarks_estimator.hpp"
#include "results_marker.hpp"

struct ScreenCalibration {
  cv::Point2f topLeft;
  cv::Point2f topRight;
  cv::Point2f bottomRight;
  cv::Point2f bottomLeft;
};

// Overload the << operator to print the points for logging
inline std::ostream &operator<<(std::ostream &os,
                                const ScreenCalibration &calibration) {
  os << "Top-left: " << calibration.topLeft << ", "
     << "Top-right: " << calibration.topRight << ", "
     << "Bottom-right: " << calibration.bottomRight << ", "
     << "Bottom-left: " << calibration.bottomLeft;
  return os;
}

class VisionGuard {
public:
  VisionGuard(const std::string &gaze_model, const std::string &face_model,
              const std::string &head_pose_model,
              const std::string &landmarks_model,
              const std::string &eye_state_model, const std::string &device);
  ~VisionGuard();

  void setCalibrationPoints(const ScreenCalibration &calibrationPoints);
  void setCalibrationPoints(const cv::Point2f &topLeft,
                            const cv::Point2f &topRight,
                            const cv::Point2f &bottomRight,
                            const cv::Point2f &bottomLeft);
  bool checkGazeTimeExceeded() const;
  void createEmptyDataFile();
  void defaultCalibration(const cv::Size &imageSize);
  std::vector<std::string> getAvailableDevices();
  std::map<std::string, double> getDailyStats();
  double getAccumulatedGazeTime() const;
  std::string getHourlyTimestamp() const;
  double getGazeLostDuration() const;
  std::map<std::string, double> getWeeklyStats();
  bool isDeviceAvailable(const std::string &device);
  bool isToggled(char toggleType) const;
  void logGazeData();
  void processFrame(cv::Mat &frame);
  void resetGazeTime();
  void saveJsonData(const nlohmann::json &data, const std::string &filePath);
  void
  setAccumulatedGazeTimeThreshold(const double accumulated_gaze_time_threshold);
  void setGazeLostThreshold(const double gazeLostThreshold);
  void toggle(int key);
  cv::Point2f captureGazePoint(cv::Mat &frame);

private:
  void cleanOldData();
  ScreenCalibration getDefaultCalibrationPoints(const cv::Size &imageSize);
  bool isGazeInScreen(const ScreenCalibration &calibration,
                      const cv::Point3f &gazeVector, const cv::Size &imageSize);
  nlohmann::json readDataFile();
  void updateGazeTime(
      const gaze_estimation::FaceInferenceResults &faceInferenceResults,
      const cv::Size &imageSize);
  void updateHourlyData(nlohmann::json &data, const std::string &key,
                        double value);

  // Gaze time tracking
  double accumulatedGazeTime = 0; // In seconds
  double accumulated_gaze_time_threshold = 20*60; // In min
  bool isGazingAtScreen = false;
  double gazeLostThreshold = 10;
  std::chrono::steady_clock::time_point gazeLostTime;
  std::chrono::steady_clock::time_point lastCheckTime;

  // Data handling
  std::string dataFilePath;

  std::string getDataFilePath() {
    QString dataLocation =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(dataLocation);
    return (dataLocation + "/screen_time_stats.json").toStdString();
  }

  // Calibration
  ScreenCalibration calibration;
  cv::Point2f convertGazeVectorToPointRel(
      const cv::Point3f &gazeVector, const cv::Size &screenSize,
      float cameraHeight = 0.05f, // 5cm above screen by default
      float screenWidth = 0.34f,  // 34cm wide by default (15.6" laptop)
      float screenHeight = 0.19f);

  // OpenVINO components
  ov::Core core;
  gaze_estimation::FaceDetector faceDetector;
  gaze_estimation::GazeEstimator gazeEstimator;
  gaze_estimation::HeadPoseEstimator headPoseEstimator;
  gaze_estimation::LandmarksEstimator landmarksEstimator;
  gaze_estimation::EyeStateEstimator eyeStateEstimator;
  gaze_estimation::ResultsMarker resultsMarker;
  std::vector<gaze_estimation::BaseEstimator *> estimators;

  // Performance metrics
  PerformanceMetrics metrics;

  // Presenter
  Presenter presenter;
  std::unordered_map<char, bool> toggleStates;

  // Timing
  std::chrono::steady_clock::time_point start_time;
};