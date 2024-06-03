#!/bin/bash

# Set up OpenVINO environment variables
source /Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/setupvars.sh
export OpenVINO_DIR=/Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/runtime/cmake
export OpenCV_DIR=/Users/inbasekaranperumal/Developer/OpenSource/build_opencv

# # Navigate to the build directory or create it if it doesn't exist
# mkdir -p /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/build
# cd /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/build

# # Run CMake to configure and generate the build system
# cmake -DCMAKE_BUILD_TYPE=Release /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/backend

# # Build the project
# cmake --build . --config Release --parallel $(sysctl -n hw.ncpu)