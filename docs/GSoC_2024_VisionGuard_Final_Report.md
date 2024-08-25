# VisionGuard: Google Summer of Code 2024 Project

[![GitHub](https://img.shields.io/badge/GitHub-100000?style=flat&logo=github&logoColor=white)](https://github.com/inbasperu/VisionGuard)

VisionGuard is a cross-platform desktop application developed as part of [Google Summer of Code 2024](https://summerofcode.withgoogle.com/programs/2024) under the [OpenVINO Toolkit](https://github.com/openvinotoolkit). Designed to reduce eye strain, it uses real-time gaze tracking with customizable break reminders, screen time analytics, and multi-hardware support (CPU/GPU/NPU), all while ensuring privacy-focused processing. Built in C++, the application integrates OpenVINO, Qt, and OpenCV to deliver efficient performance with a user-friendly interface.

For more information, visit: [VisionGuard](https://github.com/inbasperu/VisionGuard).

## Project Goals

1. Develop a privacy-focused screen time management tool using OpenVINO’s gaze detection models.
2. Provide a real-time interface for eye gaze tracking and break notifications.
3. Optimize inference for multiple hardware platforms.
4. Deliver screen time statistics while ensuring user privacy.

## Key Achievements

1. **Real-time Eye Gaze Tracking**: Uses five [OpenVINO model zoo](https://github.com/openvinotoolkit/open_model_zoo) models([face detection](https://docs.openvino.ai/2024/omz_models_model_face_detection_retail_0005.html), [head pose estimation](https://docs.openvino.ai/2024/omz_models_model_head_pose_estimation_adas_0001.html), [facial landmarks](https://docs.openvino.ai/2024/omz_models_model_facial_landmarks_35_adas_0002.html), [eye state estimation](https://docs.openvino.ai/2022.3/omz_models_model_open_closed_eye_0001.html), [gaze estimation](https://docs.openvino.ai/2024/omz_models_model_gaze_estimation_adas_0002.html)), available in FP32, FP16, and INT8-FP16.
2. **Customizable Break Reminders**: Implements the 20-20-20 rule for eye strain reduction.
3. **Screen Time Analytics**: Tracks daily and weekly usage statistics.
4. **Flexible Device Support**: Enables efficient real-time inference on CPU, GPU, and NPU.
5. **Multi-Camera Compatibility**: Supports up to five cameras.
6. **User Interface Features**: Offers dark/light themes, system resource monitoring, and frame processing optimization.
7. **System Tray Integration**: Provides seamless background operation with quick access to key features.
8. **Pre-built Binaries**: Available for Windows x64 and macOS (arm64).

## Current State

VisionGuard is fully functional with its core features of gaze tracking, break reminders, and screen time insights. Pre-built binaries can be found on the [GitHub releases page](https://github.com/inbasperu/VisionGuard/releases) for Windows and macOS.

## Key Components

### 1. Client

- **Main Window Application**: Contains UI elements for Gaze Vector Display, Calibration Window, Screen Time Widget, and Statistics Window.
- **System Tray Application**: Allows non-intrusive background operation and control.

### 2. Backend

- **Core Logic**: Manages data flow and communication between client and backend.
- **Gaze Detection Engine**: Processes video frames for gaze estimation using OpenVINO model zoo.
- **Gaze Vector Calibration**: Adjusts and fine-tunes the gaze vector range through convex hull algorithms and error margin correction to ensure accurate gaze mapping on the screen.
- **Eye Gaze Time Tracker**: Tracks screen gaze duration and break intervals using ray casting algorithm to determine the intersection of gaze vector with the screen.
- **Break Notification System**: Triggers alerts based on the 20-20-20 rule.
- **Metric and Performance Calculators**: Computes usage metrics, inference latency, frame rate, and system resource consumption.
- **Statistics Calculator**: Aggregates user analytics for display.

### 3. Data Storage

- **Usage Metrics**: Stores data for screen time, break intervals, and long-term trends.

For detailed technical information, see the [VisionGuard Architecture File](ARCHITECTURE.md).

## Future Work

1. Expand support to Linux distributions (Debian, Fedora, Arch).
2. Implement unit testing for improved reliability.
3. Develop GitHub workflows for CI/CD.
4. Support multi-monitor setups and multi-user environments.

## Code Upstream Status

VisionGuard is listed under the [Computer Vision](https://github.com/openvinotoolkit/awesome-openvino?tab=readme-ov-file#ai-computer-vision) section of [Awesome OpenVINO](https://github.com/openvinotoolkit/awesome-openvino). Releases are available on the [GitHub releases page](https://github.com/inbasperu/VisionGuard/releases).

## Challenges and Learnings

1. **Cross-Platform Development**: Overcame compatibility issues across OS platforms.
2. **CMake Implementation**: Gained insights into build configurations and dependency management.
3. **Low-Level Design**: Applied modern C++ principles to enhance performance.
4. **Screen Calibration**: Developed an intuitive process using computational geometry.
5. **Hands-On Learning**: Gained experience with OpenVINO, OpenCV, and Qt6, and how they function across different OS environments.

## Project Resources

- [GitHub Repository](https://github.com/inbasperu/VisionGuard)
- [Demo Video](https://drive.google.com/file/d/1BQvB53rj8PuewHOuJtZKygDKW3mm-Hcr/view?usp=sharing)

### Presentations

- [Final Evaluation (23 Aug 2024)](https://docs.google.com/presentation/d/1hep7HIdMWntDBDMxAbn5csEWBKlcDmHSBTJDUiGLRKM/edit?usp=sharing)
- [Midterm Evaluation (12 July 2024)](https://docs.google.com/presentation/d/1R3WkxIgNpJlb9n-94gUwp73liy82-74WYVHmW8Y8cqg/edit?usp=sharing)
- [Initial Presentation (24 May 2024)](https://docs.google.com/presentation/d/15u_2WrPaPXJ6oqB-wJcef3S57LvqdKizgIaRHG8vmPs/edit?usp=sharing)

### Proposal

- [Submitted Proposal](https://docs.google.com/document/d/1F15tOu4DOktz0_ZpZnye3QpyYvtAEpevkHoD7FNx3Ns/edit?usp=sharing)

### Development Plan

- [Gantt Chart and Plan](https://docs.google.com/spreadsheets/d/1CfnZK7eUM7_uEG4tkpvwrrlmc7pPPsqp847bD5i0BZQ/edit?usp=sharing)

## Acknowledgments

Special thanks to mentors [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov) and [Ria Cheruvu](https://github.com/riacheruvu) for their guidance. Thanks to the OpenVINO community and [Abhishek Nandy](https://www.linkedin.com/in/abhishek-nandy) for his AI-PC, which facilitated benchmarking. The benchmarking process is detailed [here](https://github.com/inbasperu/VisionGuard/blob/main/docs/BENCHMARKING.md).
