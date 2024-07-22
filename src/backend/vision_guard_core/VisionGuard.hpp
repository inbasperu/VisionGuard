#ifndef VISIONGUARD_HPP
#define VISIONGUARD_HPP

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "break_notification_system.hpp"
#include "eye_gaze_time_tracker.hpp"
#include "gaze_detection_engine.hpp"
#include "performance_tracker.hpp"
#include "screen_time_metric_logger.hpp"
#include "vector_calibration.hpp"

class VisionGuard {
public:
    VisionGuard(const std::string &gaze_model, const std::string &face_model,
                const std::string &head_pose_model,
                const std::string &landmarks_model,
                const std::string &eye_state_model, const std::string &device);
    ~VisionGuard();

    void processFrame(cv::Mat &frame);
    void toggle(int key);

private:
    GazeDetectionEngine gazeDetectionEngine;
    VectorCalibration vectorCalibration;
    EyeGazeTimeTracker eyeGazeTimeTracker;
    BreakNotificationSystem breakNotificationSystem;
    PerformanceTracker performanceTracker;
    ScreenTimeMetricLogger screenTimeMetricLogger;
};

#endif // VISIONGUARD_HPP
