// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <chrono>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>

#include "face_inference_results.hpp"

namespace gaze_estimation {

struct ScreenCalibration {
  std::vector<cv::Point2f> points; // Calibration points on the screen
  cv::Rect screenBounds;           // Bounding box of the screen
  bool isCalibrated = false;
};
class ResultsMarker {
public:
  ResultsMarker(bool showFaceBoundingBox, bool showHeadPoseAxes,
                bool showLandmarks, bool showGaze, bool showEyeState);
  void mark(cv::Mat &image, const FaceInferenceResults &faceInferenceResults);
  void toggle(int key);
  void showCalibrationWindow(std::vector<cv::Point2f> &calibrationPoints,
                             const cv::Size &imageSize);
  void calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints);
  void updateGazeTime(const cv::Point3f &gazeVector, const cv::Size &imageSize);

  // Getters
  bool getFaceBoundingBoxToggle();
  bool getHeadPoseAxesToggle();
  bool getLandmarksToggle();
  bool getGazeToggle();
  bool getEyeStateToggle();

  bool getCalibrationStatus();

  // Setters
  void setFaceBoundingBoxToggle(bool showFaceBoundingBox);
  void setHeadPoseAxesToggle(bool showHeadPoseAxes);
  void setLandmarksToggle(bool showLandmarks);
  void setGazeToggle(bool showGaze);
  void setEyeStateToggle(bool showEyeState);

private:
  bool showFaceBoundingBox;
  bool showHeadPoseAxes;
  bool showLandmarks;
  bool showGaze;
  bool showEyeState;
  ScreenCalibration calibration;
  std::chrono::steady_clock::time_point lastCheckTime;
  std::chrono::steady_clock::time_point gazeLostTime;
  double accumulatedGazeTime = 0; // In seconds
  bool isGazingAtScreen = false;
};
} // namespace gaze_estimation
