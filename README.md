# VisionGuard

VisionGuard is a desktop application designed to help prevent eye strain from excessive screen time. It uses your PC's webcam to monitor how long you look at the screen and reminds you to take breaks. This approach helps reduce eye fatigue and promotes healthier computer usage habits. The app is user-friendly, allowing you to customize your break times and providing reports on your screen time. It operates efficiently by leveraging the AI PC's Neural Processing Unit (NPU), which eliminates the need for an internet connection and keeps power usage low. This ensures that VisionGuard is both private and practical for daily use, without slowing down your computer. All your data remains on your PC, safeguarding your privacy.

## VisionGuard Deliverables

### Back Development

- [x] Implement Gaze Detection Model
- [x] Calibrate the Gaze Vector Range
- [x] Estimate Eye Gaze Time
- [x] Alert User for Breaks through Notifications (20-20-20)
- [x] Integrate the GUI
- [x] Utilize NPU and GPU
- [x] Customizable Settings (Interval and Duration)
- [x] Screen Time Reports
  - [x] Daily
  - [x] Weekly
- [x] Perform Reports
  - [x] Latency
  - [x] FPS
  - [x] Resource Utilization Graphs
- [ ] Perform Optimizations
- [ ] Handle Multi-Monitor Setup
- [ ] Handle Multi-Users

### GUI Development

- [x] Real-time User Gaze Vector Display
- [x] Gaze Calibration Window
- [x] Ongoing Screen Time Widget
- [x] Notification Alert Message Box
- [x] Customizable Settings (Interval and Duration)
- [x] Screen Time Reports
  - [x] Daily
  - [x] Weekly
- [x] Perform Reports
  - [x] Latency
  - [x] FPS
  - [x] Resource Utilization Graphs
- [ ] Display device (CPU, NPU, GPU)
- [ ] Display model (INT8, FP16, FP32)
- [ ] Add AUTO to the list of devices and test the application with AUTO.
- [ ] Run inference at user-specified FPS
- [ ] Windows system tray application
- [ ] Adaptive UI
- [ ] Dark and Light Theme Support
- [ ] Handle Multi-Monitor Setup
- [ ] Handle Multi-Users

### Benchmarking

- [ ] CPU vs. GPU vs. NPU
  - [ ] Power Consumption
  - [ ] Resource Utilization
  - [ ] Performance Metrics

### Documentation

- [ ] Setup and Development Process
- [ ] Gaze Detection and Calibration Steps
- [ ] New Features and Optimizations
- [ ] User Guides for Initial and Advanced Features
- [ ] Benchmarking Results

### User Testing

- [x] Initial User Testing and Feedback Collection
- [ ] Iterative Improvements Based on User Feedback
- [ ] Final User Testing and Validation

### Bugs

- [ ] Investigate why NPU is always active regardless of the selected device.
- [ ] Resolve discrepancies in CPU resource utilization between the application and Task Manager.
- [ ] Fix the GUI issue to ensure numbers are visible and indicate units (minutes and seconds).
- [ ] Provide feedback when "calibrate" is clicked.

### Research

- [ ] Propose an approach to test the accuracy of the solution.
- [ ] Research how accuracy depends on FPS and estimate the optimal FPS value for minimal power consumption.
- [ ] Review the provided procedure to measure power consumption.

### Deployment Deliverables

- [ ] Create an executable and deploy on Windows.
- [ ] Create an executable and deploy on Linux (Ubuntu, Fedora, Arch).
- [ ] Create an executable and deploy on MacOS.
- [ ] Set up GitHub workflows for CI/CD.

### Suggested GitHub Workflows

1. **Continuous Integration (CI) Workflow:**
   - **Build:** Ensure the application builds correctly on different operating systems (Windows, Linux, MacOS).
   - **Test:** Run unit tests and integration tests to validate the application.
   - **Lint:** Check the code for styling and potential errors using tools like `clang-tidy` for C++ and `flake8` for Python scripts (if any).

2. **Continuous Deployment (CD) Workflow:**
   - **Release:** Automatically create releases and deploy executables to GitHub Releases when new tags are pushed.
   - **Packaging:** Package the application for different platforms and ensure the executables are uploaded to the release.

3. **Code Quality Workflow:**
   - **Static Analysis:** Run static analysis tools to check for code quality and potential bugs.

4. **Documentation Workflow:**
   - **Generate Documentation:** Automatically generate and deploy documentation.

## Project Status

This project is under development as a part of GSoC 2024 under OpenVINO. For more details, visit the [GSoC Project Page](https://summerofcode.withgoogle.com/programs/2024/projects/QUbIeRAM).

## Links

- [Gaze Estimation Demo](https://docs.openvino.ai/2024/omz_demos_gaze_estimation_demo_cpp.html)
- [VisionGuard Project Gantt Chart](https://docs.google.com/spreadsheets/d/1CfnZK7eUM7_uEG4tkpvwrrlmc7pPPsqp847bD5i0BZQ/edit?usp=sharing)

## Contributors

- **Contributor:** [Inbasekaran](https://github.com/inbasperu)
- **Mentors:** [ZhuoWu](https://github.com/zhuo-yoyowz), [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov), [Ria Cheruvu](https://github.com/riacheruvu)
- **Organization:** [OpenVINO Toolkit](https://github.com/openvinotoolkit)

## License

This project is licensed under the Apache 2.0 License - see the LICENSE file for details.
