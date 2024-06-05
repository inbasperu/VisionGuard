#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  // visionGuard = new VisionGuard();
  timer = new QTimer(this);

  connect(ui->Calibrate, &QPushButton::clicked, this,
          &MainWindow::on_Calibrate_clicked);
  // connect(ui->startButton, &QPushButton::clicked, this,
  // &MainWindow::on_startButton_clicked);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
}

MainWindow::~MainWindow() {
  delete ui;
  // delete visionGuard;
}

void MainWindow::on_Calibrate_clicked() {
  // visionGuard->calibrate();
  if (!cap.isOpened()) {
    cap.open(0); // Open default camera
  }
  timer->start(33); // 30 FPS
}

void MainWindow::on_startButton_clicked() {
  if (!cap.isOpened()) {
    cap.open(0); // Open default camera
  }
  timer->start(33); // 30 FPS
}

void MainWindow::updateFrame() {
  cv::Mat frame;
  cap >> frame; // Capture a new frame from the camera

  if (frame.empty())
    return;

  // visionGuard->processFrame(frame);

  // Resize the frame to 700x428
  cv::Mat resizedFrame;
  cv::resize(frame, resizedFrame, cv::Size(700, 428));

  // Convert the resized frame to QImage
  cv::cvtColor(resizedFrame, resizedFrame,
               cv::COLOR_BGR2RGB); // Convert BGR to RGB
  QImage img =
      QImage((const unsigned char *)(resizedFrame.data), resizedFrame.cols,
             resizedFrame.rows, QImage::Format_RGB888);
  ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(img));

  QTime time{QTime::currentTime()};
  QString time_text{time.toString("mm : ss")};
  ui->GazeTime->setText(time_text);
  ui->GazeLostTime->setText("00 : 00");

  // ui->GazeTime->setText(QString::number(visionGuard->getGazeTime()));
  // ui->GazeLostTime->setText(QString::number(visionGuard->getGazeAwayTime()));
}
