#include "vector_calibration.hpp"
#include <opencv2/highgui/highgui.hpp>

VectorCalibration::VectorCalibration() : isCalibratedFlag(false) {}

void VectorCalibration::calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints) {
    points = calibrationPoints;
    isCalibratedFlag = true;
}

void VectorCalibration::showCalibrationWindow(const cv::Size &imageSize) {
    const int numPoints = 5;
    cv::Mat calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
    std::vector<cv::Point2f> calibrationPoints = getDefaultCalibrationPoints(imageSize, numPoints);

    for (int i = 0; i < numPoints; ++i) {
        calibrationImage = cv::Mat::zeros(imageSize, CV_8UC3);
        cv::circle(calibrationImage, calibrationPoints[i], 10, cv::Scalar(0, 255, 0), -1);
        cv::imshow("Calibration", calibrationImage);
        cv::waitKey(1000); // Wait for 1 second
    }
    cv::destroyWindow("Calibration");
    calibrateScreen(calibrationPoints);
}

std::vector<cv::Point2f> VectorCalibration::getDefaultCalibrationPoints(const cv::Size &imageSize, const int numPoints) {
    std::vector<cv::Point2f> calibrationPoints;
    // Define calibration points (corners and center)
    calibrationPoints.push_back(cv::Point2f(0.1 * imageSize.width, 0.1 * imageSize.height)); // Top-left
    calibrationPoints.push_back(cv::Point2f(0.9 * imageSize.width, 0.1 * imageSize.height)); // Top-right
    calibrationPoints.push_back(cv::Point2f(0.5 * imageSize.width, 0.5 * imageSize.height)); // Center
    calibrationPoints.push_back(cv::Point2f(0.1 * imageSize.width, 0.9 * imageSize.height)); // Bottom-left
    calibrationPoints.push_back(cv::Point2f(0.9 * imageSize.width, 0.9 * imageSize.height)); // Bottom-right
    return calibrationPoints;
}

bool VectorCalibration::isCalibrated() const {
    return isCalibratedFlag;
}

bool VectorCalibration::isPointInsidePolygon(const std::vector<cv::Point2f> &polygon, const cv::Point2f &point) const {
    int i, j, nvert = polygon.size();
    bool c = false;
    for (i = 0, j = nvert - 1; i < nvert; j = i++) {
        if (((polygon[i].y >= point.y) != (polygon[j].y >= point.y)) &&
            (point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
                            (polygon[j].y - polygon[i].y) +
                        polygon[i].x)) {
            c = !c;
        }
    }
    return c;
}
