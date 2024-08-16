# VisionGuard Features and Usage

## Key Features

- **Real-time Eye Gaze Tracking:** Monitors your screen time with precise gaze tracking. All processing is done locally, ensuring your privacy and data security.
- **Customizable Break Notifications:** Reminds you to take breaks at customizable intervals, following the 20-20-20 rule or other user-defined settings.
- **Daily and Weekly Statistics:** Provides detailed statistics on your screen time, stored locally and automatically cleared after one week.
- **Device Switching:** Easily switch between CPU, GPU, and NPU devices to optimize performance based on your setup.
- **Multiple Capture Devices:** Supports switching between up to five connected camera devices for flexibility in use.
- **Dark and Light Themes:** Offers customization of the interface with dark or light themes according to your preference.
- **Resource Utilization Monitor:** Monitors and displays system resource usage to ensure the application runs efficiently.
- **Optimized for Performance:** Allows switching between different quantized weights to balance performance and resource efficiency.
- **Frame Processing Limitation:** Limits the number of frames processed to optimize resource usage and performance.
- **System Tray Application:** Runs in the system tray, providing quick access to key features without cluttering your desktop.
- **Robust Architecture:** Built with C++ for memory efficiency, using Qt6, OpenCV, and OpenVINO to ensure high performance and extensibility.

## Run-Time Control Keys

During runtime, VisionGuard allows you to control the displayed information using the following keyboard shortcuts:

- **G**: Toggle the display of the gaze vector.
- **B**: Toggle the display of face detector bounding boxes.
- **O**: Toggle the display of head pose information.
- **L**: Toggle the display of facial landmarks.
- **E**: Toggle the display of eye state (open/closed).
- **A**: Display all inference results.
- **N**: Hide all inference results.
- **F**: Flip frames horizontally.
- **Command + Q**: Quit the application.

### Resource Monitor Keys

The resource monitor can be controlled using these keys:

- **C**: Display CPU average usage.
- **D**: Display CPU distribution.
- **M**: Display memory usage.

## Usage Instructions

1. **Launching VisionGuard:**
   - After installation, launch VisionGuard from the Start menu or desktop shortcut.
   - Ensure your webcam is connected before starting the application.

2. **Customizing Settings:**
   - Navigate to the settings menu to customize break intervals, theme preferences, and device configurations.

3. **Viewing Statistics:**
   - Access daily and weekly screen time statistics from the main interface. These are automatically cleared after one week.

4. **System Tray Access:**
   - VisionGuard runs in the system tray. Right-click the tray icon to access key features quickly without opening the full application.
