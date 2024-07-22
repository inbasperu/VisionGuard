#ifndef GAZE_DETECTION_ENGINE_HPP
#define GAZE_DETECTION_ENGINE_HPP

#include "face_detector.hpp"
#include "gaze_estimator.hpp"
#include "head_pose_estimator.hpp"
#include "landmarks_estimator.hpp"
#include "eye_state_estimator.hpp"
#include "results_marker.hpp"
#include <opencv2/core.hpp>

class GazeDetectionEngine {
public:
    GazeDetectionEngine(const std::string &face_model,
                        const std::string &head_pose_model,
                        const std::string &landmarks_model,
                        const std::string &eye_state_model,
                        const std::string &device, ov::Core &core);

    void processFrame(cv::Mat &frame);

    // Additional methods as needed

private:
    gaze_estimation::FaceDetector faceDetector;
    gaze_estimation::GazeEstimator gazeEstimator;
    gaze_estimation::HeadPoseEstimator headPoseEstimator;
    gaze_estimation::LandmarksEstimator landmarksEstimator;
    gaze_estimation::EyeStateEstimator eyeStateEstimator;
    gaze_estimation::ResultsMarker resultsMarker;
};

#endif // GAZE_DETECTION_ENGINE_HPP
