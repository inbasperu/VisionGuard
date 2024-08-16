#pragma once

#include "vision_guard.hpp"
#include <QCloseEvent>
#include <QIcon>
#include <QMainWindow>
#include <QMenu>
#include <QMessageBox>
#include <QPermissions>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QValueAxis>
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
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private slots:
  void quitApplication();
  void checkGazeTime();
  void on_Calibrate_clicked();
  void on_actionExit_triggered();
  void on_breakDurationHorizontalSlider_valueChanged(int value);
  void on_breakDurationSpinBox_valueChanged(int arg1);
  void on_breakIntervalHorizontalSlider_valueChanged(int value);
  void on_breakIntervalSpinBox_valueChanged(int arg1);
  void on_dailyStatButton_clicked();
  void on_resourceUtilizationButton_clicked();
  void on_weeklyStatButton_clicked();
  void on_FPSLimitSpinBox_valueChanged(int value);
  void on_FPSLimitHorizontalSlider_valueChanged(int value);
  void on_resetCalibration_clicked();
  void on_setMargin_clicked();

private:
  void initializeAfterPermission();
  void createTrayIcon();
  void createActions();
  void setIcon();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);

  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;
  QAction *minimizeAction;
  QAction *maximizeAction;
  QAction *restoreAction;
  QAction *quitAction;

protected:
  void closeEvent(QCloseEvent *event) override;

private:
  void performFourPointCalibration();
  void displayChart(const std::map<std::string, double> &stats,
                    const QString &title);
  std::string getResourcePath(const std::string &resourceName);
  std::string getModelPath(const std::string &modelName,
                           const std::string &precision);
  std::unique_ptr<VisionGuard>
  initializeVisionGuard(const std::string &precision,
                        const std::string &device);
  void loadModels(const std::string &precision);
  void populateCameraMenu();
  void updateCameraMenu();
  void initializeUI();
  void initializeVisionGuard();
  std::vector<int> getAvailableCameras(int maxCameras);
  QAction *createCameraAction(int cameraIndex);
  void switchCamera(int cameraIndex);
  bool requestCameraPermission();
  void populateDeviceMenu();
  void updateDeviceMenu();
  void populateModelMenu();
  void updateModelMenu();
  void populateToggleMenu();
  void updateToggleMenu();
  void switchDevice(const std::string &device);
  void updateFrame();
  std::vector<cv::Point2f> captureGazePoints(int durationMs);
  std::vector<cv::Point2f> calculateCalibrationPoints(
      const std::vector<std::vector<cv::Point2f>> &allGazePoints,
      int errorMargin);

  Ui::MainWindow *ui;
  QTimer *timer;
  std::unique_ptr<VisionGuard> visionGuard;
  std::unique_ptr<ImagesCapture> cap;

  double errorMargin = -10;
  void setCalibrationErrorMargin(double margin);
  double getErrroMargin() { return errorMargin; };

  static bool quitting;
  static bool first_quit;
  // Device and Precision
  std::string currentDevice;
  std::string currentPrecision;
  int currentCameraIndex;

  // Performance Metrics
  PerformanceMetrics metrics;

  // Frame Settings
  int FPS_LIMIT = 25;
  std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime;
  int frameIntervalMs = 1000 / FPS_LIMIT;
  cv::Size imageSize = cv::Size(1920, 1080);

  // Break duration (seconds)
  // TODO: change it to 20 min after testing
  int gazeDuration = 20;
  // Break interval (seconds)
  // TODO: change it to 20 sec after testing
  int intervalDuration = 10;

  // Path to image icon png
  QString iconPath;

  // Model base paths
  const std::string MODELS_DIR = "omz_models";
  const std::string GAZE_MODEL_NAME = "gaze-estimation-adas-0002";
  const std::string FACE_MODEL_NAME = "face-detection-retail-0004";
  const std::string HEAD_POSE_MODEL_NAME = "head-pose-estimation-adas-0001";
  const std::string LANDMARKS_MODEL_NAME = "facial-landmarks-35-adas-0002";
  const std::string EYE_STATE_MODEL_NAME = "open-closed-eye-0001";
  const std::string MODEL_EXTENSION = ".xml";

  // Precision levels
  const std::string FP32_PRECISION = "FP32";
  const std::string FP16_PRECISION = "FP16";
  const std::string INT8_PRECISION = "FP16-INT8";

  // Toggle keys
  const char TOGGLE_ALL = 'A';
  const char TOGGLE_EYE_STATE = 'E';
  const char TOGGLE_FACE_BOUNDING_BOX = 'B';
  const char TOGGLE_GAZE = 'G';
  const char TOGGLE_HEAD_POSE_AXES = 'O';
  const char TOGGLE_LANDMARKS = 'L';
  const char TOGGLE_NONE = 'N';
  const char TOGGLE_RESOURCE_GRAPH = 'H';
};
