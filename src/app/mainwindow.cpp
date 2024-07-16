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

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#else
#include <limits.h>
#include <unistd.h>
#endif

/**
 * @brief MainWindow constructor.
 * @param parent The parent widget.
 */
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

  populateDeviceMenu();
  populateCameraMenu();

  connect(ui->breakDurationSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationSpinBox_valueChanged(int)));
  connect(ui->breakDurationHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationHorizontalSlider_valueChanged(int)));
  connect(ui->breakIntervalSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalSpinBox_valueChanged(int)));
  connect(ui->breakIntervalHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalHorizontalSlider_valueChanged(int)));
}

/**
 * @brief MainWindow destructor.
 */
MainWindow::~MainWindow() {
  delete ui;
  delete timer;
  visionGuard.reset();
}

/**
 * @brief Checks if the gaze time exceeds the threshold and alerts the user.
 */
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

/**
 * @brief Displays a chart with gaze time statistics.
 * @param stats The statistics data.
 * @param title The title of the chart.
 */
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

  chart->legend()->setVisible(true);
  chart->legend()->setFont(QFont("Arial", 10));

  QDialog dialog(this);
  dialog.setWindowTitle(title);
  dialog.resize(800, 600);

  QVBoxLayout *layout = new QVBoxLayout(&dialog);
  QLabel *label = new QLabel("Gaze Time Statistics", &dialog);
  QPushButton *okButton = new QPushButton("OK", &dialog);
  connect(okButton, &QPushButton::clicked, &dialog, &QDialog::accept);

  layout->addWidget(label);
  layout->addWidget(chartView);
  layout->addWidget(okButton);

  dialog.exec();
}

/**
 * @brief Fetches the executable path.
 * @return The path to the executable.
 */
std::string MainWindow::getExecutablePath() {
  char buffer[PATH_MAX];

#if defined(_WIN32) || defined(_WIN64)
  GetModuleFileNameA(nullptr, buffer, PATH_MAX);
  std::string::size_type pos = std::string(buffer).find_last_of("\\/");
  return std::string(buffer).substr(0, pos);
#elif defined(__APPLE__)
  uint32_t size = PATH_MAX;
  if (_NSGetExecutablePath(buffer, &size) != 0) {
    throw std::runtime_error("Failed to get executable path");
  }
  std::string path(buffer);
  std::string::size_type pos = path.find_last_of('/');
  return path.substr(0, pos);
#else
  ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
  if (count == -1) {
    throw std::runtime_error("Failed to get executable path");
  }
  buffer[count] = '\0';
  std::string path(buffer);
  std::string::size_type pos = path.find_last_of('/');
  return path.substr(0, pos);
#endif
}

/**
 * @brief Retrieves the model path.
 * @param modelName The name of the model.
 * @param precision The precision level of the model.
 * @return The full path to the model.
 */
std::string MainWindow::getModelPath(const std::string &modelName,
                                     const std::string &precision) {
  std::string modelPath = getExecutablePath() + "/" + MODELS_DIR + "/" +
                          modelName + "/" + precision + "/" + modelName +
                          MODEL_EXTENSION;

  if (!std::filesystem::exists(modelPath)) {
    slog::err << "Model file not found: " << modelPath << slog::endl;
    throw std::runtime_error("Model file not found: " + modelPath);
  }

  slog::debug << "Found " << modelName << " at " << modelPath << " with "
              << precision << " precision" << slog::endl;
  return modelPath;
}

/**
 * @brief Initializes the VisionGuard object.
 * @param precision The precision level for the models.
 * @param device The device to run the models on.
 * @return A unique pointer to the VisionGuard object.
 */
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

/**
 * @brief Handles key press events.
 * @param event The key event.
 */
void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    close();
  } else {
    visionGuard->toggle(event->key());
  }
}

/**
 * @brief Loads models with the specified precision.
 * @param precision The precision level for the models.
 */
void MainWindow::loadModels(const std::string &precision) {
  currentPrecision = precision;
  visionGuard.reset();
  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
}

void MainWindow::on_actionExit_triggered() { close(); }

void MainWindow::on_actionFP16_triggered() { loadModels(FP16_PRECISION); }

void MainWindow::on_actionFP32_triggered() { loadModels(FP32_PRECISION); }

void MainWindow::on_actionINT8_triggered() { loadModels(INT8_PRECISION); }

void MainWindow::on_actionShow_All_triggered() {
  visionGuard->toggle(TOGGLE_ALL);
}

void MainWindow::on_actionShow_Eye_State_triggered() {
  visionGuard->toggle(TOGGLE_EYE_STATE);
}

void MainWindow::on_actionShow_Face_Bounding_Box_triggered() {
  visionGuard->toggle(TOGGLE_FACE_BOUNDING_BOX);
}

void MainWindow::on_actionShow_Gaze_triggered() {
  visionGuard->toggle(TOGGLE_GAZE);
}

void MainWindow::on_actionShow_Head_Pose_Axes_triggered() {
  visionGuard->toggle(TOGGLE_HEAD_POSE_AXES);
}

void MainWindow::on_actionShow_Landmarks_triggered() {
  visionGuard->toggle(TOGGLE_LANDMARKS);
}

void MainWindow::on_actionShow_None_triggered() {
  visionGuard->toggle(TOGGLE_NONE);
}

void MainWindow::on_breakDurationHorizontalSlider_valueChanged(int value) {
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(value));
  ui->breakDurationSpinBox->setValue(value);
}

void MainWindow::on_breakDurationSpinBox_valueChanged(int arg1) {
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(arg1));
  ui->breakDurationHorizontalSlider->setValue(arg1);
}

void MainWindow::on_breakIntervalHorizontalSlider_valueChanged(int value) {
  visionGuard->setGazeLostThreshold(static_cast<double>(value));
  ui->breakIntervalSpinBox->setValue(value);
}

void MainWindow::on_breakIntervalSpinBox_valueChanged(int arg1) {
  visionGuard->setGazeLostThreshold(static_cast<double>(arg1));
  ui->breakIntervalHorizontalSlider->setValue(arg1);
}

void MainWindow::on_Calibrate_clicked() {
  visionGuard->defaultCalibration(this->imageSize);
}

void MainWindow::on_dailyStatButton_clicked() {
  auto dailyStats = visionGuard->getDailyStats();
  displayChart(dailyStats, "Daily Gaze Time Stats");
}

void MainWindow::on_resourceUtilizationButton_clicked() {
  visionGuard->toggle(TOGGLE_RESOURCE_GRAPH);
}

void MainWindow::on_weeklyStatButton_clicked() {
  auto weeklyStats = visionGuard->getWeeklyStats();
  displayChart(weeklyStats, "Weekly Gaze Time Stats");
}

/**
 * @brief Populates the camera menu with available camera devices.
 */
void MainWindow::populateCameraMenu() {
  ui->menuCamera->clear();
  std::vector<int> availableCameras;
  for (int i = 0; i < 5; ++i) {
    try {
      cv::VideoCapture cap(i);
      if (cap.isOpened()) {
        availableCameras.push_back(i);
        cap.release();
        slog::info << "Found camera at index " << i << slog::endl;
      } else {
        slog::warn << "Camera at index " << i << " failed to open."
                   << slog::endl;
      }
    } catch (const cv::Exception &e) {
      continue;
    } catch (const std::exception &e) {
      slog::err << "Standard exception while checking camera at index " << i
                << ": " << e.what() << slog::endl;
    } catch (...) {
      slog::err << "Unknown exception while checking camera at index " << i
                << slog::endl;
    }
  }
  for (int cameraIndex : availableCameras) {
    QString cameraName = QString("Camera %1").arg(cameraIndex);
    QAction *cameraAction = new QAction(cameraName, this);
    connect(cameraAction, &QAction::triggered, this,
            [this, cameraIndex]() { switchCamera(cameraIndex); });
    ui->menuCamera->addAction(cameraAction);
  }
}

/**
 * @brief Populates the device menu with available devices.
 */
void MainWindow::populateDeviceMenu() {
  ui->menuDevices->clear();
  std::vector<std::string> availableDevices =
      visionGuard->getAvailableDevices();
  for (const auto &device : availableDevices) {
    QAction *deviceAction = new QAction(QString::fromStdString(device), this);
    connect(deviceAction, &QAction::triggered, this,
            [this, device]() { switchDevice(device); });
    ui->menuDevices->addAction(deviceAction);
  }
}

/**
 * @brief Switches to the selected camera.
 * @param cameraIndex The index of the camera.
 */
void MainWindow::switchCamera(int cameraIndex) {
  // TODO: Add your logic to switch to the selected camera
  // Example: reinitialize the VisionGuard with the new camera index
  // visionGuard->setCameraIndex(cameraIndex);
  // visionGuard.reset();
  // visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
  // Restart the capture or other necessary operations
}

/**
 * @brief Switches to the selected device.
 * @param device The device to switch to.
 */
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

/**
 * @brief Updates the frame from the camera.
 */
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