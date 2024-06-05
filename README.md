# VisionGuard

VisionGuard is a desktop application designed to help prevent eye strain from excessive screen time. It uses your PC's webcam to monitor how long you look at the screen and reminds you to take breaks. This approach helps reduce eye fatigue and promotes healthier computer usage habits. The app is user-friendly, allowing you to customize your break times and providing reports on your screen time. It operates efficiently by leveraging the AI PC's Neural Processing Unit (NPU), which eliminates the need for an internet connection and keeps power usage low. This ensures that VisionGuard is both private and practical for daily use, without slowing down your computer. All your data remains on your PC, safeguarding your privacy.

## VisionGuard Deliverables

### Back Development
- [x] Implement Gaze Detection Model
- [x] Calibrate the Gaze Vector Range
- [x] Estimate Eye Gaze Time
- [ ] Alert User for Breaks through Notifications (20-20-20)
- [ ] Integrate the GUI
- [ ] Utilize NPU and GPU
- [ ] Perform Optimizations
- [ ] Customizable Settings (Interval and Duration)
- [ ] Screen Time Reports
  - [ ] Daily
  - [ ] Weekly
  - [ ] Monthly
- [ ] Handle Multi-Monitor Setup
- [ ] Handle Multi-Users

### GUI Development
- [x] Real-time User Gaze Vector Display
- [x] Gaze Calibration Window
- [x] Ongoing Screen Time Widget
- [ ] Notification Alert Message Box
- [ ] Customizable Settings Sliders (Interval and Duration)
- [ ] Adaptive UI
- [ ] Screen Time Reports
  - [ ] Daily
  - [ ] Weekly
  - [ ] Monthly
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
- [ ] Initial User Testing and Feedback Collection
- [ ] Iterative Improvements Based on User Feedback
- [ ] Final User Testing and Validation

## Project Status
This project is under development as a part of GSoC 2024 under OpenVINO. For more details, visit the [GSoC Project Page](https://summerofcode.withgoogle.com/programs/2024/projects/QUbIeRAM).

## Links
- [Gaze Estimation Demo](https://docs.openvino.ai/2024/omz_demos_gaze_estimation_demo_cpp.html)
- [VisionGuard Project Gantt Chart](https://docs.google.com/spreadsheets/d/1CfnZK7eUM7_uEG4tkpvwrrlmc7pPPsqp847bD5i0BZQ/edit?usp=sharing)

## Contributors
- **Mentors:** [ZhuoWu](https://github.com/zhuo-yoyowz), [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov), [Ria Cheruvu](https://github.com/riacheruvu)
- **Organization:** [OpenVINO Toolkit](https://github.com/openvinotoolkit)

## License
This project is licensed under the Apache 2.0 License - see the LICENSE file for details.
