#!/bin/bash

# Run the application
./build/arm64/Release/gaze_estimation_demo -d CPU -i 0 \
-m /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/models/intel/gaze-estimation-adas-0002/FP32/gaze-estimation-adas-0002.xml \
-m_fd /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/models/intel/face-detection-retail-0004/FP32/face-detection-retail-0004.xml \
-m_hp /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/models/intel/head-pose-estimation-adas-0001/FP32/head-pose-estimation-adas-0001.xml \
-m_lm /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/models/intel/facial-landmarks-35-adas-0002/FP32/facial-landmarks-35-adas-0002.xml \
-m_es /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/models/public/open-closed-eye-0001/FP32/open-closed-eye-0001.xml