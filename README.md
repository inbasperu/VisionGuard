<p align="center">
  <img src="resources/vision-guard-removebg.png" alt="VisionGuard Logo" width="100" height="100"/>
</p>

<h1 align="center">VisionGuard</h1>

<p align="center">
  <strong>Protect Your Eyes, Enhance Your Productivity</strong>
</p>

<p align="center">
  <a href="#features">Features</a> •
  <a href="#installation">Installation</a> •
  <a href="#usage">Usage</a> •
  <a href="#development">Development</a> •
  <a href="docs/ARCHITECTURE.md">Architecture</a> •
  <a href="docs/BENCHMARKING.md">Benchmarks</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a> •
  <a href="docs/BLOG_DRAFT.md">Blog</a>
</p>

<p align="center">
  <img src="https://upload.wikimedia.org/wikipedia/commons/a/a7/GSoC-logo-horizontal.svg" alt="Google Summer of Code Logo" height="60" style="margin-right: 20px;"/>
  <img src="https://upload.wikimedia.org/wikipedia/commons/4/45/OpenVINO_logo.svg" alt="OpenVINO Logo" height="60"/>
</p>

## About VisionGuard

VisionGuard is an innovative desktop application designed to combat eye strain resulting from prolonged screen time. Leveraging advanced computer vision technology, it utilizes your PC's webcam to monitor screen time and provide timely break reminders, promoting healthier computing habits and reducing eye fatigue.

<p align="center">
  <img src="docs/screen_gaze.png" alt="Gaze Tracking Illustration" width="400"/>
</p>

### Key Advantages

- **Privacy-Focused:** Operates entirely locally, requiring no internet access.
- **Data Security:** All data remains on your device, ensuring your privacy.
- **Performance Optimized:** Supports inference on AI PC's Neural Processing Unit (NPU) for enhanced efficiency.

This project was developed as part of Google Summer of Code 2024 under the [OpenVINO Toolkit](https://github.com/openvinotoolkit), mentored by [Dmitriy Pastushenkov](https://github.com/DimaPastushenkov), [Ria Cheruvu](https://github.com/riacheruvu), and [ZhuoWu](https://github.com/zhuo-yoyowz). For more details, visit the [GSoC Project Page](https://summerofcode.withgoogle.com/programs/2024/projects/QUbIeRAM).

## Features

- **Real-time Eye Gaze Tracking:** Accurate, privacy-preserving screen time monitoring.
- **Customizable Break Notifications:** Smart alerts based on personalized intervals.
- **Comprehensive Statistics:** Daily and weekly screen time insights.
- **Flexible Device Support:** Seamless switching between CPU, GPU, and NPU.
- **Multi-Camera Compatibility:** Support for up to five camera devices.
- **Aesthetic Customization:** Dark and light theme options.
- **Resource Optimization:** Built-in system resource monitor and frame processing limits.
- **Convenient System Tray Integration:** Quick access without desktop clutter.

For an in-depth look at these features, please refer to our [Features Documentation](docs/FEATURES.md).

## Installation

Get started with VisionGuard by following our step-by-step [Installation Guide](docs/INSTALLATION.md).

## Usage

Learn how to make the most of VisionGuard's features with our comprehensive [User Guide](docs/USAGE.md).

## Development

Interested in contributing or building VisionGuard from source? Our [Development Guide](docs/DEVELOPMENT.md) provides all the information you need to get started.

### Architecture

For a deep dive into VisionGuard's technical architecture, including backend processes and gaze estimation models, consult our [Architecture Documentation](docs/ARCHITECTURE.md).

### Benchmarking

We've conducted extensive performance benchmarks across various devices. View our findings in the [Benchmarking Documentation](docs/BENCHMARKING.md).

## Contributing

We welcome contributions from the community! Before submitting your pull request, please review our [Contributing Guidelines](docs/CONTRIBUTING.md).

## Reporting Issues

Encountered a bug or have a suggestion? We value your feedback! Please open an [issue](https://github.com/inbasperu/VisionGuard/issues) in our GitHub repository.

## License

VisionGuard is dual-licensed:

- Core application code (non-Qt): Apache License 2.0
- Qt-dependent components: GNU Lesser General Public License (LGPL) version 3

For full details, refer to our [License File](LICENSE.md).

## Acknowledgements

- **Lead Developer:** [Inbasekaran](https://github.com/inbasperu)
- **Mentors:** Dmitriy Pastushenkov, Ria Cheruvu, ZhuoWu
- **OpenVINO Toolkit Community**

## Blog

Stay updated with our latest developments and insights on our [Blog](docs/BLOG_DRAFT.md).

---

<p align="center">
  Made with ❤️ for healthier computing habits
</p>
