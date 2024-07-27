#ifndef VECTOR_CALIBRATION_HPP
#define VECTOR_CALIBRATION_HPP

#include <opencv2/core.hpp>
#include <vector>

class VectorCalibration {
public:
  VectorCalibration(const cv::Size &screenSize);

  // Getters
  bool isCalibrated() const;

  // Calibration methods
  void calibrateScreen(const std::vector<cv::Point2f> &calibrationPoints);

private:
  // Helper methods
  std::vector<cv::Point2f>
  getDefaultCalibrationPoints();
  bool isPointInsidePolygon(const std::vector<cv::Point2f> &polygon,
                            const cv::Point2f &point) const;
  void showCalibrationWindow();

  // Data members
  std::vector<cv::Point2f> points;
  bool isCalibratedFlag;
  cv::Size screenSize;
};

#endif // VECTOR_CALIBRATION_HPP
