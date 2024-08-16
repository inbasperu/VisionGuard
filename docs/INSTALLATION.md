# VisionGuard Installation Guide

## Windows Installation

### Installation Instructions

#### Method 1: Using the Installer

1. **Download the Installer:**
   - Obtain the `VisionGuardSetupWindowsIntelx64.exe` file from the official download link.

2. **Run the Installer:**
   - Double-click the downloaded installer file and follow the on-screen instructions to complete the installation process.

3. **Launch the Application:**
   - After installation, you can launch VisionGuard from the Start menu or the desktop shortcut.

---

#### Method 2: Manual Installation (Using Zip File)

1. **Download and Extract:**
   - Download the `WindowsIntelx64.zip` file from the official source.
   - Extract the contents of the zip file to a preferred location on your system.

2. **Running the Application:**
   - Navigate to the extracted folder and double-click `VisionGuardApp.exe` to start the application.

---

**Important:** Ensure your webcam is connected before launching VisionGuard, as it is required for video capture. All processing and inference are performed locally on your machine, ensuring that no data is transmitted externally, thereby protecting your privacy.

---

### Prerequisites

- **Microsoft Visual C++ Redistributable:**
   - Install the latest version of the [Microsoft Visual C++ Redistributable](https://learn.microsoft.com/en-us/cpp/windows/latest-supported-vc-redist?view=msvc-170#latest-microsoft-visual-c-redistributable-version) appropriate for your system architecture from the official Microsoft website.

- **NPU Drivers:**
   - Ensure that your Neural Processing Unit (NPU) drivers are up to date.