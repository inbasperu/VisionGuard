#!/bin/bash

# Run the application
./build/arm64/Release/gaze_estimation_demo -d CPU -i 0 \
-m ./models/intel/gaze-estimation-adas-0002/FP32/gaze-estimation-adas-0002.xml \
-m_fd ./models/intel/face-detection-retail-0004/FP32/face-detection-retail-0004.xml \
-m_hp ./models/intel/head-pose-estimation-adas-0001/FP32/head-pose-estimation-adas-0001.xml \
-m_lm ./models/intel/facial-landmarks-35-adas-0002/FP32/facial-landmarks-35-adas-0002.xml \
-m_es ./models/public/open-closed-eye-0001/FP32/open-closed-eye-0001.xml