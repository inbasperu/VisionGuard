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

  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
  slog::debug << "VisionGuard backend initialized successfully" << slog::endl;

  cap = openImagesCapture("0", false, read_type::efficient, 0,
                          std::numeric_limits<size_t>::max(),
                          stringToSize("1280x720"));
  slog::debug << "Capture device initialized successfully" << slog::endl;
  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
  connect(timer, &QTimer::timeout, this, &MainWindow::checkGazeTime);
  timer->start(30);

  // Connect sliders and spin boxes to their respective slots
  connect(ui->breakDurationSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationSpinBox_valueChanged(int)));
  connect(ui->breakDurationHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationHorizontalSlider_valueChanged(int)));
  connect(ui->breakIntervalSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalSpinBox_valueChanged(int)));
  connect(ui->breakIntervalHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalHorizontalSlider_valueChanged(int)));
}

MainWindow::~MainWindow() {
  delete ui;
  delete timer;
  visionGuard.reset();
}

std::string MainWindow::getModelPath(const std::string &modelName,
                                     const std::string &precision) {
  std::string modelPath{BASE_PATH + "/" + modelName + "/" + precision + "/" +
                        modelName + MODEL_EXTENSION};
  slog::debug << "Reading " << modelName << " from " << modelPath << " with "
              << precision << " precision" << slog::endl;
  return modelPath;
}

std::unique_ptr<VisionGuard>
MainWindow::initializeVisionGuard(const std::string &precision,
                                  const std::string &device) {
  auto guard = std::make_unique<VisionGuard>(
      getModelPath(GAZE_MODEL_NAME, precision),
      getModelPath(FACE_MODEL_NAME, precision),
      getModelPath(HEAD_POSE_MODEL_NAME, precision),
      getModelPath(LANDMARKS_MODEL_NAME, precision),
      getModelPath(EYE_STATE_MODEL_NAME, precision), device);
  guard->defaultCalibration(this->imageSize);
  return guard;
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
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    close();
  } else {
    visionGuard->toggle(event->key());
  }
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
    visionGuard.reset();
    visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
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
  visionGuard.reset();
  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
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

void MainWindow::on_breakDurationSpinBox_valueChanged(int arg1) {
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(arg1));
  ui->breakDurationHorizontalSlider->setValue(
      arg1); // Synchronize slider with spin box
}

void MainWindow::on_breakDurationHorizontalSlider_valueChanged(int value) {
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(value));
  ui->breakDurationSpinBox->setValue(value); // Synchronize spin box with slider
}

void MainWindow::on_breakIntervalSpinBox_valueChanged(int arg1) {
  visionGuard->setGazeLostThreshold(static_cast<double>(arg1));
  ui->breakIntervalHorizontalSlider->setValue(
      arg1); // Synchronize slider with spin box
}

void MainWindow::on_breakIntervalHorizontalSlider_valueChanged(int value) {
  visionGuard->setGazeLostThreshold(static_cast<double>(value));
  ui->breakIntervalSpinBox->setValue(value); // Synchronize spin box with slider
}

void MainWindow::on_dailyStatButton_clicked()
{

}


void MainWindow::on_weeklyStatButton_clicked()
{

}

