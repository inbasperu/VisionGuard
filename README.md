# VisionGuard

VisionGuard is a desktop application designed to help prevent eye strain from excessive screen time. Utilizing your PC's webcam, it monitors screen time and provides timely reminders to take breaks, promoting healthier computer usage habits and reducing eye fatigue. The application operates entirely locally, requiring no internet access, and ensures your data remains on your device, safeguarding your privacy. VisionGuard also supports inference on the AI PC's Neural Processing Unit (NPU), optimizing performance and efficiency.

This project was developed as part of GSoC 2024 under the [OpenVINO Toolkit](https://github.com/openvinotoolkit), mentored by [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov), [Ria Cheruvu](https://github.com/riacheruvu), and [ZhuoWu](https://github.com/zhuo-yoyowz). For more details, visit the [GSoC Project Page](https://summerofcode.withgoogle.com/programs/2024/projects/QUbIeRAM).

## Features and Usage

- **Real-time Eye Gaze Tracking:** Monitors screen time with accurate gaze tracking, processing everything locally to ensure privacy and data security.
- **Customizable Break Notifications:** Alerts you to take breaks based on customizable intervals, following guidelines like the 20-20-20 rule.
- **Daily and Weekly Statistics:** Provides detailed screen time statistics stored locally, with automatic clearing after one week.
- **Device Switching:** Seamlessly switch between CPU, GPU, and NPU devices for optimal performance.
- **Multiple Capture Devices:** Easily switch between up to five connected camera devices.
- **Dark and Light Themes:** Customize the interface with dark or light themes.
- **Resource Utilization Monitor:** Tracks system resource usage to ensure efficient performance.
- **Optimized for Performance:** Allows switching between different quantized weights to balance performance and efficiency.
- **Frame Processing Limitation:** Controls the maximum number of frames processed to optimize resource efficiency.
- **System Tray Application:** Runs in the system tray, giving quick access to key features without cluttering your desktop.
- **Robust Architecture:** Built with C++ for memory efficiency, leveraging Qt6, OpenCV, and OpenVINO for extensibility and high performance.

For more details on these features and usage, visit the [Features Documentation](docs/FEATURES.md).

## Installation

To install VisionGuard, follow the steps in our [Installation Guide](docs/INSTALLATION.md).

## Development

Interested in contributing or building the application from source? Check out our [Development Guide](docs/DEVELOPMENT.md).

## Architecture

To understand the architecture behind VisionGuard, including backend processes and gaze estimation models, refer to the [Architecture Documentation](docs/ARCHITECTURE.md).

## Benchmarking

We have conducted benchmarks on different devices to evaluate VisionGuard's performance. For detailed results and methodology, see our [Benchmarking Documentation](docs/BENCHMARKING.md).

## Reporting Issues

We value your feedback! Please report any issues or suggest improvements by opening an [issue](https://github.com/inbasperu/VisionGuard/issues) in our GitHub repository.

## Contributing

We welcome community contributions. Please read our [Contributing Guidelines](docs/CONTRIBUTING.md) before submitting pull requests.

## License

VisionGuard is dual-licensed. The core application code, which does not directly use Qt, is licensed under the Apache License 2.0. Parts of the application that directly use Qt are licensed under the GNU Lesser General Public License (LGPL) version 3. For more details, see the [License File](LICENSE.md).

## Contributors

- **Contributor:** [Inbasekaran](https://github.com/inbasperu)