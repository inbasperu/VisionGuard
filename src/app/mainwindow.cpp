#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QTime>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <algorithm>
#include <filesystem>
#include <windows.h>

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

std::string MainWindow::getModelPath(const std::string &modelName, const std::string &precision) {
    std::string modelPath = getExecutablePath() + "/" + MODELS_DIR + "/" + modelName + "/" + precision + "/" + modelName + MODEL_EXTENSION;

    // Check if the model file exists
    if (!std::filesystem::exists(modelPath)) {
        slog::err << "Model file not found: " << modelPath << slog::endl;
        throw std::runtime_error("Model file not found: " + modelPath);
    }
    
    slog::debug << "Found " << modelName << " at " << modelPath << " with " << precision << " precision" << slog::endl;
    return modelPath;
}

std::string MainWindow::getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    std::string::size_type pos = std::string(buffer).find_last_of("\\/");
    return std::string(buffer).substr(0, pos);
}

std::unique_ptr<VisionGuard>
MainWindow::initializeVisionGuard(const std::string &precision,
                                  const std::string &device) {
  // auto guard = std::make_unique<VisionGuard>(
  //     getModelPath(GAZE_MODEL_NAME, precision),
  //     getModelPath(FACE_MODEL_NAME, precision),
  //     getModelPath(HEAD_POSE_MODEL_NAME, precision),
  //     getModelPath(LANDMARKS_MODEL_NAME, precision),
  //     getModelPath(EYE_STATE_MODEL_NAME, precision), device);
  auto guard = std::make_unique<VisionGuard>(
        "C:\\Users\\Inba\\Documents\\GSoC\\VisionGuard\\build\\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\\intel64\\Debug\\..\\models\\gaze-estimation-adas-0002\\FP32\\gaze-estimation-adas-0002.xml",
        "C:\\Users\\Inba\\Documents\\GSoC\\VisionGuard\\build\\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\\intel64\\Debug\\..\\models\\face-detection-retail-0004\\FP32\\face-detection-retail-0004.xml",
        "C:\\Users\\Inba\\Documents\\GSoC\\VisionGuard\\build\\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\\intel64\\Debug\\..\\models\\head-pose-estimation-adas-0001\\FP32\\head-pose-estimation-adas-0001.xml",
        "C:\\Users\\Inba\\Documents\\GSoC\\VisionGuard\\build\\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\\intel64\\Debug\\..\\models\\facial-landmarks-35-adas-0002\\FP32\\facial-landmarks-35-adas-0002.xml",
        "C:\\Users\\Inba\\Documents\\GSoC\\VisionGuard\\build\\Desktop_Qt_6_7_1_MSVC2019_64bit-Debug\\intel64\\Debug\\..\\models\\open-closed-eye-0001\\FP32\\open-closed-eye-0001.xml",
        device
    );
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

void MainWindow::on_dailyStatButton_clicked() {
  auto dailyStats = visionGuard->getDailyStats();
  displayChart(dailyStats, "Daily Gaze Time Stats");
}

void MainWindow::on_weeklyStatButton_clicked() {
  auto weeklyStats = visionGuard->getWeeklyStats();
  displayChart(weeklyStats, "Weekly Gaze Time Stats");
}

void MainWindow::displayChart(const std::map<std::string, double> &stats,
                              const QString &title) {
  QBarSet *set = new QBarSet("Gaze Time");

  std::vector<std::string> categories;
  double totalGazeTime = 0;
  int count = 0;

  for (const auto &[key, value] : stats) {
    *set << value / 60; // Convert seconds to minutes
    categories.push_back(key);
    totalGazeTime += value / 60; // Sum gaze times in minutes
    count++;
  }

  double averageGazeTime = totalGazeTime / count;

  QBarSeries *series = new QBarSeries();
  series->append(set);

  QChart *chart = new QChart();
  chart->addSeries(series);
  chart->setTitle(title);
  chart->setAnimationOptions(QChart::SeriesAnimations);

  QStringList qCategories;
  for (const auto &category : categories) {
    qCategories << QString::fromStdString(category);
  }

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(qCategories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  double maxGazeTime = 0;
  for (int i = 0; i < set->count(); ++i) {
    maxGazeTime = std::max(maxGazeTime, set->at(i));
  }

  QValueAxis *axisY = new QValueAxis();
  axisY->setRange(0, maxGazeTime + 10);
  axisY->setTitleText("Gaze Time (minutes)");
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  // Add average line
  QLineSeries *averageLine = new QLineSeries();
  averageLine->append(0, averageGazeTime);
  averageLine->append(categories.size() - 1, averageGazeTime);
  QColor darkGreen(0, 128, 0); // Darker green color
  averageLine->setColor(darkGreen);
  averageLine->setPen(QPen(darkGreen, 2, Qt::DashLine));
  averageLine->setName("Average Gaze Time");
  chart->addSeries(averageLine);
  averageLine->attachAxis(axisX);
  averageLine->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->setRenderHint(QPainter::Antialiasing);

  // Enable legend and set font for clarity
  chart->legend()->setVisible(true);
  chart->legend()->setFont(QFont("Arial", 10));

  // Create a message box to display the chart
  QDialog dialog(this);
  dialog.setWindowTitle(title);
  dialog.resize(800, 600); // Set a standard readable size for the dialog

  QVBoxLayout *layout = new QVBoxLayout(&dialog);
  QLabel *label = new QLabel("Gaze Time Statistics", &dialog);
  QPushButton *okButton = new QPushButton("OK", &dialog);
  connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

  layout->addWidget(label);
  layout->addWidget(chartView);
  layout->addWidget(okButton);

  dialog.exec();
}
