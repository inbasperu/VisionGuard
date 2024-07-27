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

  // Getters
  bool getFaceBoundingBoxToggle();
  bool getGazeToggle();
  bool getHeadPoseAxesToggle();
  bool getLandmarksToggle();
  bool getEyeStateToggle();

  // Setters
  void setFaceBoundingBoxToggle(bool showFaceBoundingBox);
  void setGazeToggle(bool showGaze);
  void setHeadPoseAxesToggle(bool showHeadPoseAxes);
  void setLandmarksToggle(bool showLandmarks);
  void setEyeStateToggle(bool showEyeState);

  // Other member functions
  void mark(cv::Mat &image, const FaceInferenceResults &faceInferenceResults);
  void toggle(int key);

private:
  // Data members
  bool showFaceBoundingBox;
  bool showGaze;
  bool showHeadPoseAxes;
  bool showLandmarks;
  bool showEyeState;

  // Helpers
  void gazeVectorToGazeAngles(const cv::Point3f &gazeVector,
                              cv::Point2f &gazeAngles);
};

} // namespace gaze_estimation
