// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include "face_inference_results.hpp"

namespace gaze_estimation {

class BaseEstimator {
public:
  virtual ~BaseEstimator() = default;

  virtual void estimate(const cv::Mat &image,
                        FaceInferenceResults &outputResults) = 0;
};

} // namespace gaze_estimation
