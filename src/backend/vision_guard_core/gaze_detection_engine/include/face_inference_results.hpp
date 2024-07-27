// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "utils/slog.hpp"
#include <opencv2/core/core.hpp>
#include <vector>

namespace gaze_estimation {

struct FaceInferenceResults {
  float faceDetectionConfidence{};
  cv::Rect faceBoundingBox;
  std::vector<cv::Point2i> faceLandmarks;
  cv::Point3f gazeVector;
  cv::Rect leftEyeBoundingBox;
  cv::Point2f leftEyeMidpoint;
  bool leftEyeState{};
  cv::Point3f headPoseAngles;
  cv::Rect rightEyeBoundingBox;
  cv::Point2f rightEyeMidpoint;
  bool rightEyeState{};

  // Getters
  std::vector<cv::Point2f> getEyeLandmarks();

  // Friend functions
  friend slog::LogStream &
  operator<<(slog::LogStream &os,
             const FaceInferenceResults &faceInferenceResults);
};

} // namespace gaze_estimation
