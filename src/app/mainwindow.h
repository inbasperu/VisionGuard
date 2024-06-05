#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <opencv2/opencv.hpp>
// #include "VisionGuard.h"  // Include your VisionGuard header

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

private slots:
  void on_Calibrate_clicked();
  void on_startButton_clicked();
  void updateFrame();

private:
  Ui::MainWindow *ui;
  // VisionGuard *visionGuard;  // Your VisionGuard object
  QTimer *timer;
  cv::VideoCapture cap;
};

#endif // MAINWINDOW_H
