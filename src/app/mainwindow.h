#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "vision_guard.hpp"
#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void on_Calibrate_clicked();
  void on_actionExit_triggered();
  void on_actionCPU_triggered();
  void on_actionGPU_triggered();
  void on_actionNPU_triggered();
  void on_actionINT8_triggered();
  void on_actionFP16_triggered();
  void on_actionFP32_triggered();
  void on_actionShow_Landmarks_triggered();
  void on_actionShow_Head_Pose_Axes_triggered();
  void on_actionShow_Gaze_triggered();
  void on_actionShow_Face_Bounding_Box_triggered();
  void on_actionShow_Eye_State_triggered();
  void on_actionShow_All_triggered();
  void on_actionShow_None_triggered();
  void checkGazeTime();

  void on_breakDurationSpinBox_valueChanged(int arg1);

  void on_breakDurationHorizontalSlider_valueChanged(int value);

  void on_breakIntervalSpinBox_valueChanged(int arg1);

  void on_breakIntervalHorizontalSlider_valueChanged(int value);

  void on_dailyStatButton_clicked();

  void on_weeklyStatButton_clicked();

  private:
  void updateFrame();
  void switchDevice(const std::string &device);
  void loadModels(const std::string &precision);
  std::string getModelPath(const std::string &modelName,
                           const std::string &precision);
  std::unique_ptr<VisionGuard> initializeVisionGuard(const std::string &precision, const std::string &device);

  Ui::MainWindow *ui;
  std::unique_ptr<VisionGuard> visionGuard;
  QTimer *timer;
  std::unique_ptr<ImagesCapture> cap;
  std::string currentDevice;
  std::string currentPrecision;
  PerformanceMetrics metrics;
  int delay = 1;
  cv::Size imageSize = cv::Size(1920, 1080);

  // Model base paths
  const std::string BASE_PATH = "/Users/inbasekaranperumal/Developer/"
                                "OpenSource/GSoC/code/VisionGuard/models/intel";
  const std::string GAZE_MODEL_NAME = "gaze-estimation-adas-0002";
  const std::string FACE_MODEL_NAME = "face-detection-retail-0004";
  const std::string HEAD_POSE_MODEL_NAME = "head-pose-estimation-adas-0001";
  const std::string LANDMARKS_MODEL_NAME = "facial-landmarks-35-adas-0002";
  const std::string EYE_STATE_MODEL_NAME = "open-closed-eye-0001";
  const std::string MODEL_EXTENSION = ".xml";

  // Device names
  const std::string CPU_DEVICE = "CPU";
  const std::string GPU_DEVICE = "GPU";
  const std::string NPU_DEVICE = "NPU";

  // Precision levels
  const std::string FP32_PRECISION = "FP32";
  const std::string FP16_PRECISION = "FP16";
  const std::string INT8_PRECISION = "FP16-INT8";

  // Toggle keys
  const char TOGGLE_LANDMARKS = 'L';
  const char TOGGLE_HEAD_POSE_AXES = 'O';
  const char TOGGLE_GAZE = 'G';
  const char TOGGLE_FACE_BOUNDING_BOX = 'B';
  const char TOGGLE_EYE_STATE = 'E';
  const char TOGGLE_ALL = 'A';
  const char TOGGLE_NONE = 'N';
};

#endif // MAINWINDOW_H
