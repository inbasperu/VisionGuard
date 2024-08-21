# VisionGuard Development Plan

## Completed Deliverables

### Backend Development

- [x] Implemented Gaze Detection Model
- [x] Estimated Eye Gaze Time
- [x] Developed Break Notifications (20-20-20)
- [x] Integrated GUI
- [x] Leveraged NPU and GPU for enhanced performance
- [x] Added Customizable Settings for Interval and Duration
- [x] Generated Screen Time Reports (Daily, Weekly)
- [x] Performed detailed reporting (Latency, FPS, Resource Utilization)
- [x] Calibrated Gaze Vector Range
- [x] Optimized overall performance

### GUI Development

- [x] Developed Real-time User Gaze Vector Display
- [x] Added Ongoing Screen Time Widget
- [x] Implemented Notification Alert Message Box
- [x] Enabled Customizable Settings for Interval and Duration
- [x] Provided Screen Time Reports (Daily, Weekly)
- [x] Developed reporting tools for Latency, FPS, and Resource Utilization
- [x] Displayed device and model types (CPU, NPU, GPU, INT8, FP16, FP32)
- [x] Added "AUTO" device option and user-specified FPS inference
- [x] Created Gaze Calibration Window
- [x] Implemented Windows system tray application
- [x] Supported Dark and Light Themes
- [x] Designed and integrated the VisionGuard Logo

### Benchmarking

- [ ] Compare CPU, GPU, and NPU performance
  - [ ] Assess Power Consumption
  - [ ] Monitor Resource Utilization
  - [ ] Evaluate Performance Metrics

### Documentation

- [x] Document Setup and Development Process
- [x] Provide detailed Gaze Detection and Calibration Steps
- [x] Outline New Features and Optimizations
- [x] Create User Guides for Initial and Advanced Features
- [x] Compile and Present Benchmarking Results

## Future Work

### Unit Testing

- Develop and integrate comprehensive unit tests across the codebase to ensure the reliability and maintainability of the application.
  - **Focus Areas:**  
    - Accuracy of gaze detection
    - Correctness of break notification timing
    - Reliability of device switching
  - **Implementation Suggestion:** Utilize testing frameworks such as Google Test for C++ to automate and validate these tests across various environments.

### GitHub Workflows

- Automate the build, testing, and linting processes across different operating systems to streamline development and maintain high code quality.
- Integrate static analysis tools to continuously monitor and improve the codebase, ensuring adherence to best practices and early detection of potential issues.
- Handle Multi-Monitor Setup
- Handle Multi-Users

### Links

- [VisionGuard GSoC Gantt Chart](https://docs.google.com/spreadsheets/d/1CfnZK7eUM7_uEG4tkpvwrrlmc7pPPsqp847bD5i0BZQ/edit?usp=sharing)
