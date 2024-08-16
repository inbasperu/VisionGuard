#include "constants.hpp"

// Frame Settings
int delay = 1;
cv::Size imageSize = cv::Size(1920, 1080);

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
