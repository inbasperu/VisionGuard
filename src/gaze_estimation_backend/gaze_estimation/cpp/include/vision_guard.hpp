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

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "openvino/openvino.hpp"

#include <gflags/gflags.h>
#include <monitors/presenter.h>
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

  std::vector<std::string> getAvailableDevices();
  void processFrame(cv::Mat &frame);
  void showCalibrationWindow(const cv::Size &imageSize);
  void calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints);
  double getAccumulatedGazeTime() const;
  double getGazeLostDuration() const;
  void toggle(int key);

private:
  void updateGazeTime(const cv::Point3f &gazeVector, const cv::Size &imageSize);
  bool isPointInsidePolygon(const std::vector<cv::Point2f> &polygon,
                            const cv::Point2f &point) const;

  ScreenCalibration calibration;
  std::chrono::steady_clock::time_point lastCheckTime;
  std::chrono::steady_clock::time_point gazeLostTime;
  double accumulatedGazeTime = 0; // In seconds
  bool isGazingAtScreen = false;

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
