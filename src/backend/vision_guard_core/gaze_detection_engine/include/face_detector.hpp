// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <map>
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>

#include "face_inference_results.hpp"
#include "ie_wrapper.hpp"

namespace gaze_estimation {

class FaceDetector {
public:
  FaceDetector(ov::Core &core, const std::string &modelPath,
               const std::string &deviceName,
               double detectionConfidenceThreshold, bool enableReshape);
  ~FaceDetector();

  std::vector<FaceInferenceResults> detect(const cv::Mat &image);

  // Getters
  const std::string getModelType() const { return modelType; }

private:
  // Data members
  bool enableReshape;
  double detectionThreshold;
  IEWrapper ieWrapper;
  std::string inputTensorName;
  ov::Shape inputTensorDims;
  const std::string modelType = "Face Detection";
  std::size_t numTotalDetections;
  std::string outputTensorName;

  // Other member functions
  void adjustBoundingBox(cv::Rect &boundingBox) const;
};

} // namespace gaze_estimation
