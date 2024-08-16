#!/bin/bash

# Set up OpenVINO environment variables
source /Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/setupvars.sh
export OpenVINO_DIR=/Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/runtime/cmake
export OpenCV_DIR=/Users/inbasekaranperumal/Developer/OpenSource/build_opencv

# rm -rf /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/build

# Navigate to the build directory or create it if it doesn't exist
mkdir -p /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/build
cd /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/build

# Run CMake to configure and generate the build system with cache variables
cmake -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_PREFIX_PATH="/Users/inbasekaranperumal/Qt/6.7.1/macos" \
    -DOpenCV_DIR="/Users/inbasekaranperumal/Developer/OpenSource/build_opencv" \
    -DOpenVINO_DIR="/Users/inbasekaranperumal/Developer/Programs/OpenVINO-install-dir/runtime/cmake" \
    -DOMZ_MODELS_DIR="/Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard/omz_models" \
    /Users/inbasekaranperumal/Developer/OpenSource/GSoC/code/VisionGuard

# Build the project
cmake --build . --config Debug --parallel $(sysctl -n hw.ncpu)