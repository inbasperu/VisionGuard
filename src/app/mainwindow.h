#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "constants.hpp"
#include "vision_guard.hpp"
#include <QMainWindow>
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
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

protected:
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void checkGazeTime();
  void on_Calibrate_clicked();
  void on_actionExit_triggered();
  void on_actionFP16_triggered();
  void on_actionFP32_triggered();
  void on_actionINT8_triggered();
  void on_actionShow_All_triggered();
  void on_actionShow_Eye_State_triggered();
  void on_actionShow_Face_Bounding_Box_triggered();
  void on_actionShow_Gaze_triggered();
  void on_actionShow_Head_Pose_Axes_triggered();
  void on_actionShow_Landmarks_triggered();
  void on_actionShow_None_triggered();
  void on_breakDurationHorizontalSlider_valueChanged(int value);
  void on_breakDurationSpinBox_valueChanged(int arg1);
  void on_breakIntervalHorizontalSlider_valueChanged(int value);
  void on_breakIntervalSpinBox_valueChanged(int arg1);
  void on_dailyStatButton_clicked();
  void on_resourceUtilizationButton_clicked();
  void on_weeklyStatButton_clicked();

private:
  void displayChart(const std::map<std::string, double> &stats,
                    const QString &title);
  std::string getExecutablePath();
  std::string getModelPath(const std::string &modelName,
                           const std::string &precision);
  std::unique_ptr<VisionGuard>
  initializeVisionGuard(const std::string &precision,
                        const std::string &device);
  void loadModels(const std::string &precision);
  void populateCameraMenu();
  void populateDeviceMenu();
  void switchCamera(int cameraIndex);
  void switchDevice(const std::string &device);
  void updateFrame();

  Ui::MainWindow *ui;
  QTimer *timer;
  std::unique_ptr<VisionGuard> visionGuard;
  std::unique_ptr<ImagesCapture> cap;

  // Device and Precision
  std::string currentDevice;
  std::string currentPrecision;

  // Performance Metrics
  PerformanceMetrics metrics;

  // Frame Settings
  int delay = 1;
  cv::Size imageSize = DEFAULT_SCREEN_SIZE;
};

#endif // MAINWINDOW_H
