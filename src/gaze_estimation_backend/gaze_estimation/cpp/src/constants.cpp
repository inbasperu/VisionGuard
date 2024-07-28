#include "constants.hpp"

// Frame Settings
int delay = 1;
cv::Size imageSize = cv::Size(1920, 1080);

// Model base paths
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

std::vector<ToggleAction> toggleActions = {
    {"Show All", TOGGLE_ALL},
    {"Show Gaze", TOGGLE_GAZE},
    {"Show Eye State", TOGGLE_EYE_STATE},
    {"Show Landmarks", TOGGLE_LANDMARKS},
    {"Show Head Pose Axes", TOGGLE_HEAD_POSE_AXES},
    {"Show Face Bounding Box", TOGGLE_FACE_BOUNDING_BOX},
    {"Show None", TOGGLE_NONE}};
