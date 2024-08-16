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

class ResultsMarker {
public:
  ResultsMarker(bool showFaceBoundingBox, bool showHeadPoseAxes,
                bool showLandmarks, bool showGaze, bool showEyeState);
  void mark(cv::Mat &image, const FaceInferenceResults &faceInferenceResults);
  void toggle(int key);

  // Getters
  bool getFaceBoundingBoxToggle();
  bool getHeadPoseAxesToggle();
  bool getLandmarksToggle();
  bool getGazeToggle();
  bool getEyeStateToggle();

  // Setters
  void setFaceBoundingBoxToggle(bool showFaceBoundingBox);
  void setHeadPoseAxesToggle(bool showHeadPoseAxes);
  void setLandmarksToggle(bool showLandmarks);
  void setGazeToggle(bool showGaze);
  void setEyeStateToggle(bool showEyeState);

  // Helper
  void gazeVectorToGazeAngles(const cv::Point3f &gazeVector,
                              cv::Point2f &gazeAngles);

private:
  bool showFaceBoundingBox;
  bool showHeadPoseAxes;
  bool showLandmarks;
  bool showGaze;
  bool showEyeState;
};
} // namespace gaze_estimation
