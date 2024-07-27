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

class EyeStateEstimator : public BaseEstimator {
public:
  EyeStateEstimator(ov::Core &core, const std::string &modelPath,
                    const std::string &deviceName);
  ~EyeStateEstimator() override;

  void estimate(const cv::Mat &image,
                FaceInferenceResults &outputResults) override;

  // Getters
  const std::string getModelType() const { return modelType; }

private:
  // Data members
  IEWrapper ieWrapper;
  std::string inputTensorName;
  const std::string modelType = "Eye State Estimation";
  std::string outputTensorName;

  // Other member functions
  cv::Rect createEyeBoundingBox(const cv::Point2i &p1, const cv::Point2i &p2,
                                float scale = 1.8) const;
  void rotateImageAroundCenter(const cv::Mat &srcImage, cv::Mat &dstImage,
                               float angle) const;
};

} // namespace gaze_estimation
