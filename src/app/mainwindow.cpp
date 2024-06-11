#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QTime>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentDevice("CPU"),
      currentPrecision("FP32") {
  ui->setupUi(this);

  visionGuard = new VisionGuard(
      getModelPath(GAZE_MODEL_NAME, currentPrecision),
      getModelPath(FACE_MODEL_NAME, currentPrecision),
      getModelPath(HEAD_POSE_MODEL_NAME, currentPrecision),
      getModelPath(LANDMARKS_MODEL_NAME, currentPrecision),
      getModelPath(EYE_STATE_MODEL_NAME, currentPrecision), currentDevice);
  visionGuard->defaultCalibration(this->imageSize);
  slog::debug << "VisionGuard backend initialized successfully" << slog::endl;

  cap = openImagesCapture("0", false, read_type::efficient, 0,
                          std::numeric_limits<size_t>::max(),
                          stringToSize("1280x720"));
  slog::debug << "Capture device initialized successfully" << slog::endl;
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
  connect(timer, &QTimer::timeout, this, &MainWindow::checkGazeTime);
  timer->start(30);
}

MainWindow::~MainWindow() {
  delete ui;
  delete visionGuard;
}

std::string MainWindow::getModelPath(const std::string &modelName,
                                     const std::string &precision) {
  std::string modelPath{BASE_PATH + "/" + modelName + "/" + precision + "/" +
                        modelName + MODEL_EXTENSION};
  slog::debug << "Reading " << modelName << " from " << modelPath << " with "
              << precision << " precision" << slog::endl;
  return modelPath;
}

void MainWindow::on_Calibrate_clicked() {
  visionGuard->defaultCalibration(this->imageSize);
}

void MainWindow::updateFrame() {
  cv::Mat frame = cap->read();

  if (frame.empty())
    return;

  visionGuard->processFrame(frame);

  cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
  QImage img((const unsigned char *)(frame.data), frame.cols, frame.rows,
             QImage::Format_RGB888);
  ui->lastImagePreviewLabel->setPixmap(QPixmap::fromImage(img));

  QTime gazeTime(0, 0);
  gazeTime =
      gazeTime.addSecs(static_cast<int>(visionGuard->getAccumulatedGazeTime()));
  ui->GazeTime->setText(gazeTime.toString("mm:ss"));

  QTime gazeLostTime(0, 0);
  gazeLostTime = gazeLostTime.addSecs(
      static_cast<int>(visionGuard->getGazeLostDuration()));
  ui->GazeLostTime->setText(gazeLostTime.toString("mm:ss"));

  int key = cv::waitKey(delay);
  visionGuard->toggle(key);

  if (key == 27)
    close(); // Press 'Esc' to quit
}
void MainWindow::checkGazeTime() {
  if (visionGuard->checkGazeTimeExceeded()) {
    int reply = QMessageBox::question(
        this, "20-20-20 Rule Alert",
        "You have been looking at the screen for more than 20 minutes. Please "
        "look at an object 20 feet away for 20 seconds.",
        QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok) {
      visionGuard->resetGazeTime();
    }
  }
}

void MainWindow::on_actionExit_triggered() { close(); }

void MainWindow::switchDevice(const std::string &device) {
  if (visionGuard->isDeviceAvailable(device)) {
    currentDevice = device;
    delete visionGuard;
    visionGuard = new VisionGuard(
        getModelPath(GAZE_MODEL_NAME, currentPrecision),
        getModelPath(FACE_MODEL_NAME, currentPrecision),
        getModelPath(HEAD_POSE_MODEL_NAME, currentPrecision),
        getModelPath(LANDMARKS_MODEL_NAME, currentPrecision),
        getModelPath(EYE_STATE_MODEL_NAME, currentPrecision), currentDevice);
    visionGuard->defaultCalibration(this->imageSize);
  } else {
    QMessageBox::warning(
        this, "Warning",
        QString::fromStdString(device + " device is not available."));
  }
}

void MainWindow::on_actionCPU_triggered() { switchDevice(CPU_DEVICE); }
void MainWindow::on_actionGPU_triggered() { switchDevice(GPU_DEVICE); }
void MainWindow::on_actionNPU_triggered() { switchDevice(NPU_DEVICE); }

void MainWindow::loadModels(const std::string &precision) {
  currentPrecision = precision;
  delete visionGuard;
  visionGuard = new VisionGuard(
      getModelPath(GAZE_MODEL_NAME, currentPrecision),
      getModelPath(FACE_MODEL_NAME, currentPrecision),
      getModelPath(HEAD_POSE_MODEL_NAME, currentPrecision),
      getModelPath(LANDMARKS_MODEL_NAME, currentPrecision),
      getModelPath(EYE_STATE_MODEL_NAME, currentPrecision), currentDevice);
  visionGuard->defaultCalibration(this->imageSize);
}

void MainWindow::on_actionINT8_triggered() { loadModels(INT8_PRECISION); }
void MainWindow::on_actionFP16_triggered() { loadModels(FP16_PRECISION); }
void MainWindow::on_actionFP32_triggered() { loadModels(FP32_PRECISION); }

void MainWindow::on_actionShow_Landmarks_triggered() {
  visionGuard->toggle(TOGGLE_LANDMARKS);
}
void MainWindow::on_actionShow_Head_Pose_Axes_triggered() {
  visionGuard->toggle(TOGGLE_HEAD_POSE_AXES);
}
void MainWindow::on_actionShow_Gaze_triggered() {
  visionGuard->toggle(TOGGLE_GAZE);
}
void MainWindow::on_actionShow_Face_Bounding_Box_triggered() {
  visionGuard->toggle(TOGGLE_FACE_BOUNDING_BOX);
}
void MainWindow::on_actionShow_Eye_State_triggered() {
  visionGuard->toggle(TOGGLE_EYE_STATE);
}
void MainWindow::on_actionShow_All_triggered() {
  visionGuard->toggle(TOGGLE_ALL);
}
void MainWindow::on_actionShow_None_triggered() {
  visionGuard->toggle(TOGGLE_NONE);
}
