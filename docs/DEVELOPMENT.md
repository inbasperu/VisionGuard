# VisionGuard Development Guide

This guide provides detailed instructions for setting up, building, and running the VisionGuard application on your local system. It includes guidance for both Windows and Unix-based systems (macOS/Linux).

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Project Structure](#project-structure)
3. [Setup and Build Instructions](#setup-and-build-instructions)
4. [Running the Application](#running-the-application)
5. [Understanding Build Types](#understanding-build-types)
6. [Additional Notes](#additional-notes)

## Prerequisites

Before beginning the build process, ensure that the following dependencies are installed on your system:

- **Microsoft Visual C++ Redistributable (Windows only):** Required for running applications built with Visual Studio.
- **NPU Drivers:** Necessary for enabling hardware acceleration on systems equipped with a Neural Processing Unit (NPU).
- **[OpenVINO Toolkit](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_windows.html):** Provides pre-trained models and optimization tools for running inference on various devices.
- **[OpenCV](https://opencv.org/releases/):** An open-source computer vision library used for image and video processing.
- **[Qt6](https://doc.qt.io/qt-6/gettingstarted.html):** A framework for developing cross-platform applications with graphical user interfaces.

### Installation of Dependencies

- **OpenVINO:** Follow the official [OpenVINO installation guide](https://docs.openvino.ai/latest/openvino_docs_install_guides_installing_openvino_windows.html) to set up OpenVINO on your system.
- **OpenCV:** You can download and build OpenCV from the [official repository](https://github.com/opencv/opencv). Make sure to configure it for your specific platform.
- **Qt6:** Download and install Qt6 from the [official Qt website](https://www.qt.io/download). Ensure that you install the correct version that matches your platform and compiler (e.g., MSVC 2019 for Windows).

## Project Structure

The VisionGuard project has the following directory structure:

```
.
├── CMakeLists.txt
├── Info.plist
├── LICENSE-APACHE
├── LICENSE-LGPL
├── LICENSE.md
├── NOTICE.md
├── README.md
├── VisionGuardInstaller.nsi
├── build.ps1
├── build.sh
├── config.json
├── docs
├── include
├── omz_models
├── resources
├── run.ps1
├── run.sh
├── screen_time_stats.json
├── src
├── third_party
└── wiki
```

### Key Directories and Files

- **`CMakeLists.txt`**: The main CMake configuration file for building the project.
- **`docs/`**: Contains project documentation, including architecture, usage guides, and development plans.
- **`include/`**: Header files for the VisionGuard application.
- **`omz_models/`**: Pre-trained OpenVINO models used for face detection, gaze estimation, etc.
- **`resources/`**: Contains application resources like icons and QT resource files.
- **`src/`**: Source code files for the VisionGuard application.
- **`third_party/`**: Third-party libraries and dependencies.
- **`wiki/`**: Additional documentation and guides.

### Important Files

- **`build.ps1` and `build.sh`**: Build scripts for Windows and Unix systems, respectively.
- **`run.ps1` and `run.sh`**: Scripts to run the application on Windows and Unix systems.
- **`config.json`**: Configuration file for the application.
- **`screen_time_stats.json`**: File to store screen time statistics.

## Setup and Build Instructions

### Windows Setup

#### Step 1: Set Up Environment Variables

Environment variables are necessary to tell the build system where to find the installed libraries and tools. Open a terminal and set the following environment variables:

```bash
set OpenVINO_DIR=C:\Program Files (x86)\Intel\OpenVINO\runtime\cmake
set OpenCV_DIR=C:\Program Files\opencv\build
set Qt6_DIR=C:\Qt\6.7.1\msvc2019_64
```

#### Step 2: Create and Navigate to the Build Directory

Create a build directory where the compiled binaries will be stored. This keeps your source files clean and separates the build artifacts.

```bash
mkdir build
cd build
```

#### Step 3: Configure the Build System with CMake

CMake is a cross-platform build system generator. The following command configures CMake to generate a build system using Visual Studio 2019 for a 64-bit architecture:

```bash
cmake -A x64 -G "Visual Studio 16 2019" -DCMAKE_BUILD_TYPE=Release ^
      -DCMAKE_PREFIX_PATH="%Qt6_DIR%" ^
      -DOpenCV_DIR="%OpenCV_DIR%" ^
      -DOpenVINO_DIR="%OpenVINO_DIR%" ^
      -DOMZ_MODELS_DIR="C:\Users\Inba\Documents\GSoC\VisionGuard\omz_models" ^
      ..
```

- **`-A x64`**: Specifies a 64-bit build.
- **`-G "Visual Studio 16 2019"`**: Tells CMake to generate a project for Visual Studio 2019.
- **`-DCMAKE_BUILD_TYPE=Release`**: Configures the build type. Use `Release` for an optimized build or `Debug` for debugging.
- **`-DCMAKE_PREFIX_PATH`**: Specifies the paths for Qt6, OpenCV, and OpenVINO.

#### Step 4: Build the Project

After configuration, use CMake to build the project. The `--parallel` option allows you to specify the number of threads to use during the build process. Setting it to `-1` or `0` will automatically use the maximum number of available threads.

```bash
cmake --build . --config Release --parallel 0 --verbose
```

- **`--config Release`**: Specifies that this build should be optimized for performance. For a debug build, replace `Release` with `Debug`.
- **`--parallel 0`**: Utilizes the maximum number of threads available on your system to speed up the build process.
- **`--verbose`**: Outputs detailed information during the build process, which can be helpful for troubleshooting.

### Unix-Based Systems (macOS/Linux)

#### Step 1: Set Up Environment Variables

First, source the OpenVINO environment setup script to load the necessary environment variables. Then, set the variables for OpenCV and Qt6:

```bash
source /opt/intel/openvino/bin/setupvars.sh
export OpenVINO_DIR=/opt/intel/openvino/runtime/cmake
export OpenCV_DIR=/usr/local/share/opencv4
export Qt6_DIR=/opt/Qt/6.7.1/gcc_64
```

#### Step 2: Create and Navigate to the Build Directory

Create the build directory:

```bash
mkdir -p /path/to/VisionGuard/build
cd /path/to/VisionGuard/build
```

#### Step 3: Configure the Build System with CMake

Configure CMake with the following command:

```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_PREFIX_PATH="$Qt6_DIR" \
      -DOpenCV_DIR="$OpenCV_DIR" \
      -DOpenVINO_DIR="$OpenVINO_DIR" \
      -DOMZ_MODELS_DIR="/path/to/VisionGuard/omz_models" \
      /path/to/VisionGuard
```

- **`-DCMAKE_BUILD_TYPE=Debug`**: Configures the build for debugging. For an optimized release build, replace `Debug` with `Release`.

#### Step 4: Build the Project

Build the project with the following command, using all available CPU cores:

```bash
cmake --build . --config Debug --parallel 0
```

## Running the Application

Once the build process is complete, you can run the VisionGuard application:

- **Windows:** Navigate to the build directory and run `VisionGuardApp.exe`.
- **macOS/Linux:** Navigate to the build directory and run the generated executable (e.g., `./VisionGuardApp`).

## Understanding Build Types

- **Release Build:**
  - Optimized for performance.
  - Use this build type when deploying or running the application in a production environment.

- **Debug Build:**
  - Includes debug symbols and is not optimized for performance.
  - Use this build type during development when you need to debug the application.

## Additional Notes

- If you change the environment variables or CMake options, it's often necessary to clear the cache (by deleting the build directory and recreating it) to ensure that all changes are applied correctly.
- The `omz_models` directory contains pre-trained models used by VisionGuard. Ensure this directory is correctly specified during the build process.
- The `resources` directory contains essential assets for the application's user interface. Make sure these resources are accessible to the built application.
- The `third_party` directory contains external libraries and dependencies. Familiarize yourself with these components if you plan to modify or extend VisionGuard's functionality.
- Regularly check the `docs` and `wiki` directories for updated documentation and guides that may assist in development and troubleshooting.
