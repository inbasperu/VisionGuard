#ifndef VECTOR_CALIBRATION_HPP
#define VECTOR_CALIBRATION_HPP

#include <opencv2/core.hpp>
#include <vector>

class VectorCalibration {
public:
    VectorCalibration();
    void calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints);
    void showCalibrationWindow(const cv::Size &imageSize);
    std::vector<cv::Point2f> getDefaultCalibrationPoints(const cv::Size &imageSize, const int numPoints = 5);

    bool isCalibrated() const;
    bool isPointInsidePolygon(const std::vector<cv::Point2f> &polygon, const cv::Point2f &point) const;

private:
    std::vector<cv::Point2f> points;
    bool isCalibratedFlag;
};

#endif // VECTOR_CALIBRATION_HPP
