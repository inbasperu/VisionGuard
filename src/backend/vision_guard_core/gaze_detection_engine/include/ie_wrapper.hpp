// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#pragma once

#include <cstdio>
#include <map>
#include <string>
#include <vector>

#include "utils/common.hpp"
#include "utils/ocv_common.hpp"
#include "utils/slog.hpp"

namespace gaze_estimation {

class IEWrapper {
public:
  IEWrapper(ov::Core &core, const std::string &modelPath,
            const std::string &modelType, const std::string &deviceName);

  // Getters
  const std::map<std::string, ov::Shape> &getInputTensorDimsInfo() const;
  const std::map<std::string, ov::Shape> &getOutputTensorDimsInfo() const;
  std::string expectSingleInput() const;
  std::string expectSingleOutput() const;
  void expectImageInput(const std::string &tensorName) const;

  // Other member functions
  void getOutputTensor(const std::string &tensorName,
                       std::vector<float> &output);
  void infer();
  void reshape(const std::map<std::string, ov::Shape> &newTensorsDimsInfo);
  void setInputTensor(const std::string &tensorName, const cv::Mat &image);
  void setInputTensor(const std::string &tensorName,
                      const std::vector<float> &data);

private:
  // Data members
  ov::CompiledModel compiled_model;
  ov::Core core;
  std::string deviceName;
  ov::InferRequest infer_request;
  std::map<std::string, ov::Shape> input_tensors_dims_info;
  std::string modelPath;
  std::shared_ptr<ov::Model> model;
  std::string modelType;
  std::map<std::string, ov::Shape> output_tensors_dims_info;

  // Other member functions
  void setExecPart();
};

} // namespace gaze_estimation
