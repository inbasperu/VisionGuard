# VisionGuard: My Google Summer of Code 2024 Experience with OpenVINO

As my 16-week journey with Google Summer of Code (GSoC) 2024 comes to a close, I’m excited to share *VisionGuard*—a desktop application designed to reduce eye strain and promote healthier screen usage. Developed in collaboration with the [OpenVINO Toolkit](https://github.com/openvinotoolkit) team, *VisionGuard* is a personal and technical milestone that applies advanced computer vision to solve a common, everyday issue.

## The Journey to GSoC

My name is Inbasekaran Perumal, and I recently graduated from the National Institute of Technology, Surathkal (class of 2024), specializing in Electronics & Communication Engineering. Throughout my academic journey, I’ve been deeply interested in the intersection of hardware and software, particularly when it comes to integrating AI and machine learning into real-world applications. Technologies like computer vision and image processing have always captivated me, which is why OpenVINO became the perfect open-source project to align with my interests.

My introduction to OpenVINO happened during a technical session led by Intel experts, hosted by my professor, Prof. [Sumam David](https://sumam.nitk.ac.in). The workshop on neural network inference using Intel FPGAs left a lasting impression on me. That’s when I realized that contributing to OpenVINO could offer the perfect balance of learning and practical application.

I had first heard about Google Summer of Code during my freshman year, where seniors and professors often spoke about their involvement in open-source projects. While the idea of contributing to such a large global initiative initially seemed overwhelming, I was encouraged by my professors, Prof. [Mohit P. Tahiliani](https://cse.nitk.ac.in/faculty/mohit-p-tahiliani) and Prof. [Ramesh Kini](https://ece.nitk.ac.in/faculty/ramesh-kini-m), who assured me that GSoC was an excellent opportunity to grow both technically and personally. This support helped me overcome my doubts, and I finally decided to take the leap.

I hope to one day write a detailed guide for beginners interested in contributing to C++-based open-source projects, as it can be challenging at first. Such a guide could help others avoid the same struggles I faced while getting started.

## The Vision Behind VisionGuard

The idea for *VisionGuard* stemmed from a personal need. Like many, I experienced the negative effects of extended screen time, including eye strain, headaches, and disrupted sleep—especially during the remote learning period brought on by the COVID-19 pandemic. I wanted to develop a tool that not only monitors screen time but also encourages healthier screen habits.

## Project Overview

**VisionGuard** is a cross-platform desktop application designed to help users manage screen time and reduce eye strain. Developed as part of Google Summer of Code 2024 under the OpenVINO Toolkit, it leverages real-time gaze tracking, customizable break reminders, and screen time analytics—all with a focus on privacy through on-device processing.

Built using C++ and integrating OpenVINO, Qt, and OpenCV, VisionGuard optimizes for CPU, GPU, and NPU platforms, ensuring smooth performance while keeping the user experience seamless and intuitive.

More information can be found in the [VisionGuard GitHub Repository](https://github.com/inbasperu/VisionGuard).

### Key Features

- **Gaze Tracking and Calibration**  
Edge Inference which does Accurate gaze tracking built on OpenVINO model zoo gaze estimation demo for real-time feedback.

- **Customizable Break Reminders**  
   Implements a custimizaton based on 20-20-20 rule, prompting breaks every 20 minutes and the ability to set custom break duration and intervals.

- **Screen Time Analytics**  
   Detailed daily and weekly reports, offering insights into screen usage patterns.

- **Multi-Hardware Support**  
   Runs efficiently on CPU, GPU, and NPU platforms with optimized inference.

- **Multi-Camera Support**  
   Supports up to five cameras for enhanced tracking accuracy.

- **User Interface**  
   A clean, intuitive interface with light/dark themes, system resource monitoring, and frame rate optimization (to limit the maximum number of frames being processed).
   switch between quantized weithts for optimized inference available in FP32, FP16, and INT8-FP16 and also switch brtween devices

- **System Tray Integration**  
   Non-intrusive background operation, allowing easy access via system tray.

**User Data Privacy**  
   Ensure all processing occurs locally, keeping user data secure and private.

## Technical Deep Dive

The above figure illustrates the high level architecture of VisionGuard, it mainly consists of three core components the client interface, backend and the data storage. I will cover some of the important components in detail, the rest of the deatils on the architecture can be found on the [architecture](https://github.com/inbasperu/VisionGuard/blob/main/docs/ARCHITECTURE.md) documentation of VisionGuard

### Core Components

#### 1. **Client Interface**

- **Main Application Window**: Displays real-time gaze tracking, calibration settings, and screen time widgets.
- **System Tray Application**: Enables VisionGuard to run unobtrusively in the background, with easy access through the tray.

#### 2. **Backend Processing**

- **Gaze Detection Engine**: Integrates multiple OpenVINO models for real-time gaze estimation and eye tracking. This component includes:
  - [Face Detection](https://docs.openvino.ai/2024/omz_models_model_face_detection_retail_0005.html)
  - [Head Pose Estimation](https://docs.openvino.ai/2024/omz_models_model_head_pose_estimation_adas_0001.html)
  - [Facial Landmarks](https://docs.openvino.ai/2024/omz_models_model_facial_landmarks_35_adas_0002.html)
  - [Eye State Estimation](https://docs.openvino.ai/2022.3/omz_models_model_open_closed_eye_0001.html)
  - [Gaze Estimation](https://docs.openvino.ai/2024/omz_models_model_gaze_estimation_adas_0002.html)

Here is a high-level overview of the system architecture.
leverages the OpenVINO model zoo
The following networks are integral to the backend:
Starting with the face detection model
Face Detection Model: Identifies the locations of faces within an image
Head Pose Estimation Model: Estimates the head pose in Tait-Bryan angles, outputting yaw, pitch, and roll angles in degrees.
Facial Landmark Detection Model: Estimates the coordinates of facial landmarks from detected faces. Keypoints at the corners of the eyes are used to locate the eye regions required for the gaze estimation model. 
Eye State Estimation Model: Determines the open or closed state of the eyes in detected faces u
Finally the Gaze Estimation Model: Takes three inputs: square crops of the left and right eye images, and three head pose angles (yaw, pitch, and roll). It outputs a 3-D vector representing the direction of a person’s gaze in a Cartesian coordinate system

The low level system implementaton details can be found in this document https://github.com/inbasperu/VisionGuard/wiki/Gaze-Detection-Engine-Architecture

- **Gaze Calibration**:

The gaze calibration process begins with a full-screen display featuring four numbered green dots in the corners. The user looks at each dot for 1.2 seconds, during which multiple gaze points are captured for each corner. These captured points are then combined, and a convex hull algorithm determines the smallest polygon that encloses all the points. To account for potential inaccuracies in gaze tracking, the convex hull is extended by a specified error margin.
This method is used to determine the screen boundary.

- **Screen Time Tracker**: Monitors user gaze on the screen, calculating the total time spent using a ray-casting algorithm.
For gaze intersection with the previously determined screen boundary (polygon), the algorithm converts the 3D gaze vector into a 2D screen point, visualizing where the gaze intersects the screen. To determine if this point falls within a specific area, a point-in-polygon algorithm is employed. This algorithm uses a ray-casting method, where a ray is cast from the gaze point to infinity and determined if the ray intersects the screen boundary

- **Break Notification System**: Implements the custom break rule based on the famous 20-20-20 rule to remind users to take a break.

- **Performance Metrics and Statistics**: Tracks frame rates, CPU/GPU load, and system resource usage. Provides long-term data aggregation for daily and weekly screen time reports.

#### 3. **Data Management**

- **Usage Metrics**: Tracks screen time and breaks, aggregating this data for users to review trends and adjust their habits.

## Benchmarking Results

The application was tested on an M3 Pro, Intel AI-PC, Asus TUF A-15 but in this blog only the results form the NPU are discussed, for a detailled benchmark result please look at our [Beanchmarking](https://github.com/inbasperu/VisionGuard/blob/main/docs/BENCHMARKING.md) document at the VisionGuard repo.

FPS Performance

- NPU and IGPU outperform CPU, especially at higher FPS limits
- INT-8 FP-16 quantization generally improves FPS across all devices
- Higher FPS limits (25 vs 10) show significant performance improvements

Latency Performance

- IGPU consistently shows lowest latency
- INT-8 FP-16 quantization reduces latency for NPU and CPU
- 25 FPS limit doesn't significantly increase latency compared to 10 FPS

CPU Utilization

- Lowest with NPU, followed by IGPU, highest with CPU-only option
- INT-8 FP-16 quantization reduces CPU utilization
- Higher FPS increases CPU utilization across all configurations

Device-Specific Utilization

- IGPU shows highest utilization, especially at 25 FPS
- NPU demonstrates efficient operation with low utilization and high FPS
- INT-8 FP-16 quantization reduces device utilization, particularly for NPU and IGPU

Overall Takeaway

- NPU offers the best balance of performance and efficiency
- IGPU provides high performance but at the cost of high utilization
- INT-8 FP-16 quantization generally improves performance and reduces resource utilization
- Higher FPS limits showcase the capabilities of each device but increase resource utilization

## Current Status

VisionGuard is fully operational, with core features such as real-time gaze tracking, break notifications, and screen time analytics working efficiently. Pre-built binaries are available for Windows and macOS. More details, including downloads, can be found on the [GitHub Releases Page](https://github.com/inbasperu/VisionGuard/releases).

### Challenges and Learning

Throughout the GSoC experience, I encountered numerous challenges that significantly contributed to my learning and growth as a developer:

- **Cross-platform C++ Development and deployment**: Developing a cross-platform C++ application presented unique challenges, particularly in ensuring compatibility across different operating systems like macOS, Windows, and Linux. I faced difficulties with different compilers, such as issues compiling OpenVINO’s Model Zoo demo with MSVC 2022, which required troubleshooting and problem-solving to ensure smooth builds across platforms. Also learnt to deploy the app on various platforms, used NSIS for windows to ship the application into a setup file and shipped it as a DMG on mac.
  
- **Understanding and Implementing CMake**: CMake, a powerful build system, required me to deepen my understanding of build configurations and dependencies. This knowledge was essential in managing the complexity of a cross-platform project like VisionGuard.

- **Low-Level Design Issues**: Navigating C++'s low-level design complexities, particularly with object-oriented principles (OOP), was challenging. Implementing robust design patterns while maintaining performance required careful consideration of memory management and efficiency.

- **Screen Calibration for Accurate Gaze Detection**: One of the more technically demanding tasks was calibrating the screen to accurately detect if the user was gazing at the screen. This required developing a reliable and user-friendly calibration process that could adapt to different screen sizes and user positions.

- **Adhering to C++ Development Standards**: Ensuring that VisionGuard adhered to modern C++ development standards was vital for the project’s long-term maintainability. I had to revise my approach to permissions and data storage, moving from storing stats in the current working directory to using appropriate libraries for handling resources securely and efficiently.

These challenges not only helped me improve VisionGuard but also significantly enhanced my problem-solving skills and understanding of cross-platform development.

## Future Work

While I'm proud of what I've accomplished during the GSoC period, there's always room for improvement. Some areas for future development include:

1. Expand support to Linux distributions (Debian, Fedora, Arch).
2. Implement unit testing for improved reliability.
3. Develop GitHub workflows for CI/CD.
4. Support multi-monitor setups and multi-user environments.

## Conclusion

My GSoC journey with OpenVINO and VisionGuard has been an incredible learning experience. I've had the opportunity to work with cutting-edge technology, collaborate with talented mentors, and create a tool that I believe can make a real difference in people's lives.

## Acknowledgments

Special thanks to mentors [Dmitriy Pastushenkov](https://www.linkedin.com/in/dmitriy-pastushenkov) and [Ria Cheruvu](https://www.linkedin.com/in/ria-cheruvu-54348a173/overlay/about-this-profile/) for their guidance. Thanks to the OpenVINO community and [Abhishek Nandy](https://www.linkedin.com/in/abhishek-nandy) for his AI-PC, which facilitated benchmarking. Also a special thanks thanks to the OpenVINO Org Admins [Adrian Boguszewsk](https://www.linkedin.com/in/adrianboguszewski/) and [Zhuo Wu](https://www.linkedin.com/in/wuzhuo/overlay/about-this-profile/) for organizing all the demo meetings and presentations and coordinating and making the GSoC experience very smooth. Finally I would like to thanks my friends [Vaishali S](https://www.linkedin.com/in/vaishali-s-a1490a218/), [Pranav M Koundinya](https://www.linkedin.com/in/pranav-m-koundinya-4a7597207/) and [Samhita R](https://www.linkedin.com/in/samhita-r/) for testing out my application on their systems.

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

## References

[Gaze Estimation Demo OpenVINO Model Zoo](https://github.com/openvinotoolkit/open_model_zoo/blob/master/demos/gaze_estimation_demo/cpp/README.md)
 [OpenVINO Model Zoo](https://github.com/openvinotoolkit/open_model_zoo)
    [VisionGuard GitHub Repository](https://github.com/inbasperu/VisionGuard)
[OpenVINO Toolkit](https://github.com/openvinotoolkit)
 [OpenCV](https://github.com/opencv/opencv)
[qt6](https://doc.qt.io/qt-6/)
[cmake](https://cmake.org/cmake/help/latest/manual/cmake.1.html)
[20-20-20 Rule for Eye Strain](https://www.healthline.com/health/eye-health/20-20-20-rule)
[Point in Polygon Algorithm](https://en.wikipedia.org/wiki/Point_in_polygon)
[Convex Hull Algorithm](https://en.wikipedia.org/wiki/Convex_hull_algorithms)
[Tait-Bryan Angles](https://en.wikipedia.org/wiki/Euler_angles#Tait–Bryan_angles)
[NSIS](https://nsis.sourceforge.io/Simple_tutorials)
[Google Summer of Code](https://summerofcode.withgoogle.com/)
[Microsoft Visual C++](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170)
