#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <opencv2/core.hpp>
#include <string>
#include <vector>

// Frame Settings
extern int delay;
extern cv::Size imageSize;

// Model base paths
extern const std::string MODELS_DIR;
extern const std::string GAZE_MODEL_NAME;
extern const std::string FACE_MODEL_NAME;
extern const std::string HEAD_POSE_MODEL_NAME;
extern const std::string LANDMARKS_MODEL_NAME;
extern const std::string EYE_STATE_MODEL_NAME;
extern const std::string MODEL_EXTENSION;

// Precision levels
extern const std::string FP32_PRECISION;
extern const std::string FP16_PRECISION;
extern const std::string INT8_PRECISION;

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

#endif // CONSTANTS_HPP
