#pragma once

#include <opencv2/core.hpp>
#include <string>
#include <vector>

// Frame Settings
extern int delay;
extern cv::Size imageSize;

// Toggle keys
extern const char TOGGLE_ALL;
extern const char TOGGLE_EYE_STATE;
extern const char TOGGLE_FACE_BOUNDING_BOX;
extern const char TOGGLE_GAZE;
extern const char TOGGLE_HEAD_POSE_AXES;
extern const char TOGGLE_LANDMARKS;
extern const char TOGGLE_NONE;
extern const char TOGGLE_RESOURCE_GRAPH;

struct ToggleAction {
  std::string name;
  char toggleType;
};

extern std::vector<ToggleAction> toggleActions;
