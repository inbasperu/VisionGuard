// Copyright (C) 2018-2024 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//
#include <cstdio>
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "face_inference_results.hpp"
#include "results_marker.hpp"
#include <utils/ocv_common.hpp>

namespace gaze_estimation {

ResultsMarker::ResultsMarker(bool showFaceBoundingBox, bool showHeadPoseAxes,
                             bool showLandmarks, bool showGaze,
                             bool showEyeState)
    : showFaceBoundingBox(showFaceBoundingBox),
      showHeadPoseAxes(showHeadPoseAxes), showLandmarks(showLandmarks),
      showGaze(showGaze), showEyeState(showEyeState) {}
// Getters
bool ResultsMarker::getFaceBoundingBoxToggle() { return showFaceBoundingBox; }
bool ResultsMarker::getHeadPoseAxesToggle() { return showHeadPoseAxes; }
bool ResultsMarker::getLandmarksToggle() { return showLandmarks; };
bool ResultsMarker::getGazeToggle() { return showGaze; };
bool ResultsMarker::getEyeStateToggle() { return showEyeState; }

// Setters
void ResultsMarker::setFaceBoundingBoxToggle(bool showFaceBoundingBox) {
  this->showFaceBoundingBox = showFaceBoundingBox;
}
void ResultsMarker::setHeadPoseAxesToggle(bool showHeadPoseAxes) {
  this->showHeadPoseAxes = showHeadPoseAxes;
}
void ResultsMarker::setLandmarksToggle(bool showLandmarks) {
  this->showLandmarks = showLandmarks;
}
void ResultsMarker::setGazeToggle(bool showGaze) { this->showGaze = showGaze; }
void ResultsMarker::setEyeStateToggle(bool showEyeState) {
  this->showEyeState = showEyeState;
}
void ResultsMarker::mark(cv::Mat &image,
                         const FaceInferenceResults &faceInferenceResults) {
  auto faceBoundingBox = faceInferenceResults.faceBoundingBox;
  auto faceBoundingBoxWidth = faceBoundingBox.width;
  auto faceBoundingBoxHeight = faceBoundingBox.height;
  auto scale = 0.002 * faceBoundingBoxWidth;
  cv::Point tl = faceBoundingBox.tl();

  if (showFaceBoundingBox) {
    cv::rectangle(image, faceInferenceResults.faceBoundingBox,
                  cv::Scalar::all(255), 1);
    putHighlightedText(
        image,
        cv::format(
            "Detector confidence: %0.2f",
            static_cast<double>(faceInferenceResults.faceDetectionConfidence)),
        cv::Point(static_cast<int>(tl.x),
                  static_cast<int>(tl.y - 5. * faceBoundingBoxWidth / 200.)),
        cv::FONT_HERSHEY_COMPLEX, scale, cv::Scalar(200, 10, 10), 1);
  }

  if (showHeadPoseAxes) {
    auto yaw = static_cast<double>(faceInferenceResults.headPoseAngles.x);
    auto pitch = static_cast<double>(faceInferenceResults.headPoseAngles.y);
    auto roll = static_cast<double>(faceInferenceResults.headPoseAngles.z);

    auto sinY = std::sin(yaw * M_PI / 180.0);
    auto sinP = std::sin(pitch * M_PI / 180.0);
    auto sinR = std::sin(roll * M_PI / 180.0);

    auto cosY = std::cos(yaw * M_PI / 180.0);
    auto cosP = std::cos(pitch * M_PI / 180.0);
    auto cosR = std::cos(roll * M_PI / 180.0);

    auto axisLength = 0.4 * faceBoundingBoxWidth;
    auto xCenter = faceBoundingBox.x + faceBoundingBoxWidth / 2;
    auto yCenter = faceBoundingBox.y + faceBoundingBoxHeight / 2;

    // OX points from face center to camera
    // OY points from face center to right
    // OZ points from face center to up

    // Rotation matrix:
    // Yaw - counterclockwise Pitch - counterclockwise Roll - clockwise
    //     [cosY -sinY 0]          [ cosP 0 sinP]       [1    0    0 ]
    //     [sinY  cosY 0]    *     [  0   1  0  ]   *   [0  cosR sinR] =
    //     [  0    0   1]          [-sinP 0 cosP]       [0 -sinR cosR]

    //   [cosY*cosP cosY*sinP*sinR-sinY*cosR cosY*sinP*cosR+sinY*sinR]
    // = [sinY*cosP cosY*cosR-sinY*sinP*sinR sinY*sinP*cosR+cosY*sinR]
    //   [  -sinP          -cosP*sinR                cosP*cosR       ]

    // Multiply third row by -1 because screen drawing axis points down
    // Drop first row to project to a screen plane

    // OY: center to right
    cv::line(
        image, cv::Point(xCenter, yCenter),
        cv::Point(static_cast<int>(xCenter + axisLength * (cosR * cosY -
                                                           sinY * sinP * sinR)),
                  static_cast<int>(yCenter + axisLength * cosP * sinR)),
        cv::Scalar(0, 0, 255), 2);
    // OZ: center to top
    cv::line(
        image, cv::Point(xCenter, yCenter),
        cv::Point(static_cast<int>(xCenter + axisLength * (cosR * sinY * sinP +
                                                           cosY * sinR)),
                  static_cast<int>(yCenter - axisLength * cosP * cosR)),
        cv::Scalar(0, 255, 0), 2);
    // OX: center to camera
    cv::line(image, cv::Point(xCenter, yCenter),
             cv::Point(static_cast<int>(xCenter + axisLength * sinY * cosP),
                       static_cast<int>(yCenter + axisLength * sinP)),
             cv::Scalar(255, 0, 255), 2);

    putHighlightedText(
        image,
        cv::format("head pose: (y=%0.0f, p=%0.0f, r=%0.0f)", std::round(yaw),
                   std::round(pitch), std::round(roll)),
        cv::Point(static_cast<int>(faceBoundingBox.tl().x),
                  static_cast<int>(faceBoundingBox.br().y +
                                   5. * faceBoundingBoxWidth / 100.)),
        cv::FONT_HERSHEY_PLAIN, scale * 2, cv::Scalar(200, 10, 10), 1);
  }

  if (showLandmarks) {
    int lmRadius = static_cast<int>(0.01 * faceBoundingBoxWidth + 1);
    for (auto const &point : faceInferenceResults.faceLandmarks)
      cv::circle(image, point, lmRadius, cv::Scalar(0, 255, 255), -1);
  }

  if (showGaze) {
    auto gazeVector = faceInferenceResults.gazeVector;

    double arrowLength = 0.4 * faceBoundingBoxWidth;
    cv::Point2f gazeArrow;
    gazeArrow.x = gazeVector.x;
    gazeArrow.y = -gazeVector.y;

    gazeArrow = arrowLength * gazeArrow;

    // Draw eyes bounding boxes
    cv::rectangle(image, faceInferenceResults.leftEyeBoundingBox,
                  cv::Scalar::all(255), 1);
    cv::rectangle(image, faceInferenceResults.rightEyeBoundingBox,
                  cv::Scalar::all(255), 1);

    if (faceInferenceResults.leftEyeState)
      cv::arrowedLine(image, faceInferenceResults.leftEyeMidpoint,
                      faceInferenceResults.leftEyeMidpoint + gazeArrow,
                      cv::Scalar(255, 0, 0), 2);

    if (faceInferenceResults.rightEyeState)
      cv::arrowedLine(image, faceInferenceResults.rightEyeMidpoint,
                      faceInferenceResults.rightEyeMidpoint + gazeArrow,
                      cv::Scalar(255, 0, 0), 2);

    cv::Point2f gazeAngles;
    if (faceInferenceResults.leftEyeState &&
        faceInferenceResults.rightEyeState) {
      gazeVectorToGazeAngles(faceInferenceResults.gazeVector, gazeAngles);

      putHighlightedText(
          image,
          cv::format("gaze angles: (h=%0.0f, v=%0.0f)",
                     static_cast<double>(std::round(gazeAngles.x)),
                     static_cast<double>(std::round(gazeAngles.y))),
          cv::Point(static_cast<int>(faceBoundingBox.tl().x),
                    static_cast<int>(faceBoundingBox.br().y +
                                     12. * faceBoundingBoxWidth / 100.)),
          cv::FONT_HERSHEY_PLAIN, scale * 2, cv::Scalar(200, 10, 10), 1);
    }
  }

  if (showEyeState) {
    if (faceInferenceResults.leftEyeState)
      cv::rectangle(image, faceInferenceResults.leftEyeBoundingBox,
                    cv::Scalar(0, 255, 0), 1);
    else
      cv::rectangle(image, faceInferenceResults.leftEyeBoundingBox,
                    cv::Scalar(0, 0, 255), 1);

    if (faceInferenceResults.rightEyeState)
      cv::rectangle(image, faceInferenceResults.rightEyeBoundingBox,
                    cv::Scalar(0, 255, 0), 1);
    else
      cv::rectangle(image, faceInferenceResults.rightEyeBoundingBox,
                    cv::Scalar(0, 0, 255), 1);
  }
}

void ResultsMarker::toggle(int key) {
  switch (std::toupper(key)) {
  case 'L':
    setLandmarksToggle(!getLandmarksToggle());
    break;
  case 'O':
    setHeadPoseAxesToggle(!getHeadPoseAxesToggle());
    break;
  case 'G':
    setGazeToggle(!getGazeToggle());
    break;
  case 'B':
    setFaceBoundingBoxToggle(!getFaceBoundingBoxToggle());
    break;
  case 'A':
    setFaceBoundingBoxToggle(true);
    setHeadPoseAxesToggle(true);
    setLandmarksToggle(true);
    setGazeToggle(true);
    setEyeStateToggle(true);
    break;
  case 'N':
    setFaceBoundingBoxToggle(false);
    setHeadPoseAxesToggle(false);
    setLandmarksToggle(false);
    setGazeToggle(false);
    setEyeStateToggle(false);
    break;
  case 'E':
    setEyeStateToggle(!getEyeStateToggle());
    break;
  }
}

// Helper
void ResultsMarker::gazeVectorToGazeAngles(const cv::Point3f &gazeVector,
                                           cv::Point2f &gazeAngles) {
  auto r = cv::norm(gazeVector);

  double v0 = static_cast<double>(gazeVector.x);
  double v1 = static_cast<double>(gazeVector.y);
  double v2 = static_cast<double>(gazeVector.z);

  gazeAngles.x =
      static_cast<float>(180.0 / M_PI * (M_PI_2 + std::atan2(v2, v0)));
  gazeAngles.y =
      static_cast<float>(180.0 / M_PI * (M_PI_2 - std::acos(v1 / r)));
}

} // namespace gaze_estimation
