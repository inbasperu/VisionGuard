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

class LandmarksEstimator : public BaseEstimator {
public:
  LandmarksEstimator(ov::Core &core, const std::string &modelPath,
                     const std::string &deviceName);
  ~LandmarksEstimator() override;

  void estimate(const cv::Mat &image,
                FaceInferenceResults &outputResults) override;

  // Getters
  const std::string getModelType() const { return modelType; }

private:
  // Data members
  IEWrapper ieWrapper;
  std::string inputTensorName;
  const std::string modelType = "Facial Landmarks Estimation";
  size_t numberLandmarks;
  std::string outputTensorName;

  // Other member functions
  cv::Mat affineTransform(cv::Point2f center, cv::Point2f scale, float rot,
                          size_t dst_w, size_t dst_h, cv::Point2f shift,
                          bool inv);
  cv::Point2f get3rdPoint(cv::Point2f a, cv::Point2f b);
  std::vector<cv::Point2f> getMaxPreds(std::vector<cv::Mat> heatMaps);
  std::vector<cv::Point2i> heatMapPostprocess(cv::Rect faceBoundingBox,
                                              cv::Mat faceCrop);
  cv::Point2f rotatePoint(cv::Point2f pt, float angle_rad);
  int sign(float number);
  std::vector<cv::Point2i> simplePostprocess(cv::Rect faceBoundingBox,
                                             cv::Mat faceCrop);
  std::vector<cv::Mat> split(std::vector<float> &data, const ov::Shape &shape);
};

} // namespace gaze_estimation
