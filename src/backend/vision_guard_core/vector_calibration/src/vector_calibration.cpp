#include "vector_calibration.hpp"
#include <opencv2/highgui/highgui.hpp>

VectorCalibration::VectorCalibration(const cv::Size &screenSize)
    : isCalibratedFlag(false), screenSize(screenSize) {}

// Calibration method to set calibration points and mark calibration as complete
void VectorCalibration::calibrateScreen(
    const std::vector<cv::Point2f> &calibrationPoints) {
  if (calibrationPoints.size())
    points = calibrationPoints;
  else
    points = getDefaultCalibrationPoints();
  isCalibratedFlag = true;
}

// Helper method to get default calibration points (corners and center)
std::vector<cv::Point2f> VectorCalibration::getDefaultCalibrationPoints() {
  std::vector<cv::Point2f> calibrationPoints;
  // Define calibration points (corners and center)
  calibrationPoints.push_back(
      cv::Point2f(0.1 * screenSize.width, 0.1 * screenSize.height)); // Top-left
  calibrationPoints.push_back(cv::Point2f(
      0.9 * screenSize.width, 0.1 * screenSize.height)); // Top-right
  calibrationPoints.push_back(cv::Point2f(
      0.9 * screenSize.width, 0.9 * screenSize.height)); // Bottom-right
  calibrationPoints.push_back(cv::Point2f(
      0.1 * screenSize.width, 0.9 * screenSize.height)); // Bottom-left

  return calibrationPoints;
}

// Getter to check if the calibration is completed
bool VectorCalibration::isCalibrated() const { return isCalibratedFlag; }

// Checks if a point is inside a polygon using the ray-casting algorithm
bool VectorCalibration::isPointInsidePolygon(
    const std::vector<cv::Point2f> &polygon, const cv::Point2f &point) const {
  int i, j, nvert = polygon.size();
  bool c = false;

  // Iterate through all edges of the polygon
  for (i = 0, j = nvert - 1; i < nvert; j = i++) {
    // Check if the edge crosses the horizontal line at point.y and is to the
    // left of point.x
    if (((polygon[i].y >= point.y) != (polygon[j].y >= point.y)) &&
        (point.x <= (polygon[j].x - polygon[i].x) * (point.y - polygon[i].y) /
                            (polygon[j].y - polygon[i].y) +
                        polygon[i].x)) {
      // If so, toggle the inside/outside flag
      c = !c;
    }
  }

  // Return true if the point is inside the polygon, false otherwise
  return c;
}

// Helper method to display the calibration window and collect points
void VectorCalibration::showCalibrationWindow() {
  const int numPoints = calibrationPoints.size();
  cv::Mat calibrationImage = cv::Mat::zeros(screenSize, CV_8UC3);
  std::vector<cv::Point2f> calibrationPoints =
      getDefaultCalibrationPoints(screenSize, numPoints);

  for (int i = 0; i < numPoints; ++i) {
    calibrationImage = cv::Mat::zeros(screenSize, CV_8UC3);
    cv::circle(calibrationImage, calibrationPoints[i], 10,
               cv::Scalar(0, 255, 0), -1);
    cv::imshow("Calibration", calibrationImage);
    cv::waitKey(1000); // Wait for 1 second
  }
  cv::destroyWindow("Calibration");
  calibrateScreen(calibrationPoints);
}
