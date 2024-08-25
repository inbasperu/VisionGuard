# VisionGuard: Google Summer of Code 2024 Project

[![GitHub](https://img.shields.io/badge/GitHub-100000?style=flat&logo=github&logoColor=white)](https://github.com/inbasperu/VisionGuard)

VisionGuard is a cross-platform desktop application aimed at reducing eye strain through real-time gaze tracking, developed as part of [Google Summer of Code 2024](https://summerofcode.withgoogle.com/programs/2024) under the [OpenVINO Toolkit](https://github.com/openvinotoolkit).

Built entirely in C++ and leveraging the [Open Model Zoo](https://github.com/openvinotoolkit/open_model_zoo), VisionGuard provides customizable break reminders, screen time analytics, and support for various hardware (CPU/GPU/NPU). It is designed to run efficiently with local, privacy-focused processing. The application offers an intuitive user interface with system tray integration, combining the power of OpenVINO, Qt, and OpenCV for real-time performance.

To download and learn more, visit the project repository: [VisionGuard](https://github.com/inbasperu/VisionGuard).

## Project Goals

1. Develop a privacy-focused screen time management tool using OpenVINO's gaze detection capabilities.
2. Create a user-friendly interface for real-time eye gaze tracking and break notifications.
3. Implement efficient inference on various hardware, including AI PC's Neural Processing Units (NPUs).
4. Provide screen time statistics while ensuring data privacy and security.

## Key Achievements

During the GSoC period, the following key features were successfully implemented:

1. **Real-time Eye Gaze Tracking**: VisionGuard uses five different models—face detection, head pose estimation, facial landmarks detection, eye state estimation, and gaze estimation—for accurate gaze tracking. These models are available in three quantization levels: FP32, FP16, and INT8-FP16.

2. **Customizable Break Reminders**: Following the 20-20-20 rule to prompt users for breaks.

3. **Screen Time Analytics**: Daily and weekly statistics to help users manage their screen time effectively.

4. **Flexible Device Support**: Supports CPU, GPU, and NPU for real-time inference across different hardware.

5. **Multi-Camera Compatibility**: Supports up to five cameras simultaneously.

6. **User Interface Features**: Includes dark and light themes, system resource monitoring, and efficient frame processing to ensure optimal performance.

7. **System Tray Integration**: Provides quick access to key features.

8. **Pre-built Binaries**: Available for Windows x64 and macOS (arm64).

## Current State of the Project

VisionGuard is fully functional, providing gaze tracking, break reminders, and screen time insights. The latest version can be found on our [GitHub releases page](https://github.com/inbasperu/VisionGuard/releases), where pre-built binaries are available for both Windows and macOS.

## Key Components

VisionGuard's architecture is designed for efficient real-time eye-tracking and gaze monitoring. The key components are categorized into **Client**, **Backend**, and **Data Storage**, each playing a crucial role in the system's functionality.

### 1. Client

- **Main Window Application**: Provides the primary user interface (UI) for users to interact with the application. This UI includes a **Gaze Vector Display**, **Calibration Window**, **Screen Time Widget**, and a **Statistics Window**.
- **System Tray Application**: Runs as a background process in the operating system’s system tray, allowing for seamless and non-intrusive operation while the user continues their screen tasks.

The client integrates several real-time UI components to offer continuous feedback, such as displaying the current gaze vector, managing calibration inputs, and showing statistics related to screen time.

### 2. Backend

The backend is the computational core of VisionGuard, responsible for all the processing involved in gaze detection, gaze time calculation, and notification alerts. The backend components include:

- **Core Logic**: Manages the data flow between the client and backend, and orchestrates the communication between various subsystems.
- **Gaze Detection Engine**: Processes input video frames to estimate eye gaze using models from the OpenVINO model zoo.
- **Gaze Vector Calibration**: Adjusts and fine-tunes the gaze vector range through convex hull algorithms and error margin correction to ensure accurate gaze mapping on the screen.
- **Eye Gaze Time Tracker**: Monitors the duration of user gaze on the screen, updating metrics like **total screen time** and **gaze away duration**.
- **Break Notification System**: Responsible for triggering alert notifications based on the 20-20-20 rule, helping users take breaks at appropriate intervals to reduce eye strain.
- **Metric Calculator**: Computes various usage metrics including **screen gaze time**, **gaze direction**, and other statistics related to user behavior.
- **Performance Calculator**: Tracks performance metrics like **inference latency**, **frame rate**, and **CPU/GPU utilization**, providing feedback on system resource consumption.
- **Statistics Calculator**: Aggregates data for display in the Statistics Window, including total screen time, frequency of breaks, and other analytics derived from gaze tracking.

These components are optimized using the Intel OpenVINO toolkit to accelerate inference on a variety of hardware platforms.

### 3. Data Storage

- **Usage Metrics**: Stores data related to user behavior, including **screen gaze time**, **break intervals**, and detailed **usage statistics**. This data is used to calculate long-term trends and offer insights for improving user productivity and well-being.

---

For a detailed architectural overview of each component and more in-depth technical information, refer to the [VisionGuard Architecture File](ARCHITECTURE.md).

## Future Work

Although much has been achieved, there are still areas for improvement:

1. Expand support to include Linux (Debian, Fedora, and Arch).
2. Implement unit tests for better reliability and maintenance.
3. Develop GitHub workflows for automated builds, testing, and linting.
4. Add support for multi-monitor setups and multi-user environments.

## Code Upstream Status

VisionGuard has been included in the [Computer Vision](https://github.com/openvinotoolkit/open_model_zoo) section of [Awesome OpenVINO](https://github.com/openvinotoolkit/awesome-openvino). For more details, see our [pull request](https://github.com/openvinotoolkit/awesome-openvino/pull/23).

Additionally, the application’s releases and pre-releases are available on the [GitHub releases page](https://github.com/inbasperu/VisionGuard/releases).

## Challenges and Learnings

Throughout this project, several challenges were overcome, leading to valuable learnings:

1. **Cross-Platform Development**: Addressed compatibility issues across operating systems.
2. **CMake Implementation**: Gained a deeper understanding of build configurations and dependency management.
3. **Low-Level Design**: Applied modern C++ principles to ensure performance and maintainability.
4. **Screen Calibration**: Developed an intuitive calibration process using computational geometry.
5. **Hands-On Learning**: Gained experience with OpenVINO, OpenCV, and Qt6, as well as understanding how different operating systems function and the role of libraries in GUI applications.

## Project Resources

- [GitHub Repository](https://github.com/inbasperu/VisionGuard/tree/main)
- [Demo Video](https://drive.google.com/file/d/1BQvB53rj8PuewHOuJtZKygDKW3mm-Hcr/view?usp=sharing)

### Presentation Slides

- [Demo 2: Final Evaluation (23 Aug 2024)](https://docs.google.com/presentation/d/1hep7HIdMWntDBDMxAbn5csEWBKlcDmHSBTJDUiGLRKM/edit?usp=sharing)
- [Demo 1: Midterm Evaluation (12 July 2024)](https://docs.google.com/presentation/d/1R3WkxIgNpJlb9n-94gUwp73liy82-74WYVHmW8Y8cqg/edit?usp=sharing)
- [Initial Presentation (24 May 2024)](https://docs.google.com/presentation/d/15u_2WrPaPXJ6oqB-wJcef3S57LvqdKizgIaRHG8vmPs/edit?usp=share_link)

### Submitted Proposal

- [Proposal Document](https://docs.google.com/document/d/1F15tOu4DOktz0_ZpZnye3QpyYvtAEpevkHoD7FNx3Ns/edit?usp=sharing)

- [Gantt Chart and Detailed Plan](https://docs.google.com/spreadsheets/d/1CfnZK7eUM7_uEG4tkpvwrrlmc7pPPsqp847bD5i0BZQ/edit?usp=sharing)

## Acknowledgments

Heartfelt thanks to mentors [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov) and [Ria Cheruvu](https://github.com/riacheruvu) for their invaluable guidance and encouragement. Gratitude is also extended to the OpenVINO Toolkit community for their support.

A special thanks to [Abhishek Nandy](https://www.linkedin.com/in/abhishek-nandy), whose AI-PC made crucial benchmarking tests possible. His help was indispensable for the benchmarking process documented [here](https://github.com/inbasperu/VisionGuard/blob/main/docs/BENCHMARKING.md).
