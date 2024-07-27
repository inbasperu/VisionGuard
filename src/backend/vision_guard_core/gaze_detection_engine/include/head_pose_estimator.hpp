// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cstdio>
#include <string>

#include "base_estimator.hpp"
#include "face_inference_results.hpp"
#include "ie_wrapper.hpp"

namespace gaze_estimation {

class HeadPoseEstimator : public BaseEstimator {
public:
  HeadPoseEstimator(ov::Core &core, const std::string &modelPath,
                    const std::string &deviceName);
  ~HeadPoseEstimator() override;

  void estimate(const cv::Mat &image,
                FaceInferenceResults &outputResults) override;

  // Getters
  const std::string getModelType() const { return modelType; }

private:
  // Data members
  IEWrapper ieWrapper;
  std::string inputTensorName;
  const std::string modelType = "Head Pose Estimation";
};

} // namespace gaze_estimation
