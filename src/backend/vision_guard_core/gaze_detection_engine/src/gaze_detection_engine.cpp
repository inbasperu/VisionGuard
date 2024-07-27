#include "gaze_detection_engine.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <utils/slog.hpp>

GazeDetectionEngine::GazeDetectionEngine(const std::string &face_model,
                                         const std::string &head_pose_model,
                                         const std::string &landmarks_model,
                                         const std::string &eye_state_model,
                                         const std::string &device,
                                         ov::Core &core)
    : faceDetector(core, face_model, device, 0.5, false),
      gazeEstimator(core, face_model, device),
      headPoseEstimator(core, head_pose_model, device),
      landmarksEstimator(core, landmarks_model, device),
      eyeStateEstimator(core, eye_state_model, device),
      resultsMarker(false, false, false, true, true) {}

void GazeDetectionEngine::processFrame(cv::Mat &frame) {
  // Detect faces in the frame
  auto inferenceResults = faceDetector.detect(frame);

  // Process each detected face
  for (auto &inferenceResult : inferenceResults) {
    headPoseEstimator.estimate(frame, inferenceResult);
    landmarksEstimator.estimate(frame, inferenceResult);
    eyeStateEstimator.estimate(frame, inferenceResult);
    gazeEstimator.estimate(frame, inferenceResult);
  }

  // Mark the results on the frame
  for (auto const &inferenceResult : inferenceResults) {
    resultsMarker.mark(frame, inferenceResult);
  }
}
