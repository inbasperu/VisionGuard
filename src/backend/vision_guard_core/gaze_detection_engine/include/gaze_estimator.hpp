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

class GazeEstimator : public BaseEstimator {
public:
  GazeEstimator(ov::Core &core, const std::string &modelPath,
                const std::string &deviceName, bool doRollAlign = true);
  ~GazeEstimator() override;

  void estimate(const cv::Mat &image,
                FaceInferenceResults &outputResults) override;

  // Getters
  const std::string getModelType() const { return modelType; }

private:
  // Data members
  IEWrapper ieWrapper;
  const std::string modelType = "Gaze Estimation";
  std::string outputTensorName;
  bool rollAlign;

  // Other member functions
  void rotateImageAroundCenter(const cv::Mat &srcImage, cv::Mat &dstImage,
                               float angle) const;
};

} // namespace gaze_estimation
