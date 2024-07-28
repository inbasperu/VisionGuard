#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <QSize>
#include <QString>
#include <string>

#include <QString>

// File paths
const QString WINDOW_ICON_PATH = "src/assets/openvino.png";

// Stylesheets
const QString LIGHT_THEME_STYLESHEET = "";
const QString DARK_THEME_STYLESHEET =
    "QMainWindow { background-color: #2b2b2b; color: #d3d3d3; }"
    "QLabel { color: #d3d3d3; }"
    "QPushButton { background-color: #444444; color: #d3d3d3; border: 1px "
    "solid #555555; }"
    "QPushButton:hover { background-color: #555555; }"
    "QSlider::groove:horizontal { height: 4px; background: #444444; }"
    "QSlider::handle:horizontal { background: #777777; width: 10px; }"
    "QTabWidget::pane { border-top: 1px solid #555555; }"
    "QTabBar::tab { background: #444444; color: #d3d3d3; padding: 10px; }"
    "QTabBar::tab:selected { background: #555555; }";

// Screen Size
const QSize DEFAULT_SCREEN_SIZE = QSize(1920, 1080);

// Default Device type
const std::string defaultDevice = "AUTO";

// Model Directory and Names
const std::string MODELS_DIR = "../omz_models";
const std::string GAZE_MODEL_NAME = "gaze-estimation-adas-0002";
const std::string FACE_MODEL_NAME = "face-detection-retail-0004";
const std::string HEAD_POSE_MODEL_NAME = "head-pose-estimation-adas-0001";
const std::string LANDMARKS_MODEL_NAME = "facial-landmarks-35-adas-0002";
const std::string EYE_STATE_MODEL_NAME = "open-closed-eye-0001";
const std::string MODEL_EXTENSION = ".xml";

// Precision levels
const std::string FP32_PRECISION = "FP32";
const std::string FP16_PRECISION = "FP16";
const std::string INT8_PRECISION = "FP16-INT8";

// Toggle keys
const char TOGGLE_ALL = 'A';
const char TOGGLE_EYE_STATE = 'E';
const char TOGGLE_FACE_BOUNDING_BOX = 'B';
const char TOGGLE_GAZE = 'G';
const char TOGGLE_HEAD_POSE_AXES = 'O';
const char TOGGLE_LANDMARKS = 'L';
const char TOGGLE_NONE = 'N';
const char TOGGLE_RESOURCE_GRAPH = 'H';

#endif // CONSTANTS_HPP
