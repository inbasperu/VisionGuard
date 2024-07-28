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
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentDevice("AUTO"),
      currentPrecision("FP16"),
      currentCameraIndex(0) { // Initialize currentCameraIndex
  ui->setupUi(this);

  // Request camera permissions
  if (!requestCameraPermission()) {
    QMessageBox::critical(
        this, "Permission Denied",
        "VisionGuard requires camera access to function properly. Please grant "
        "camera access and restart the application.");
    QCoreApplication::quit();
    return;
  }

  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
  slog::debug << "VisionGuard backend initialized successfully" << slog::endl;

  // Open the initial capture device
  switchCamera(currentCameraIndex);

  timer = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
  connect(timer, &QTimer::timeout, this, &MainWindow::checkGazeTime);
  timer->start(33);

  populateDeviceMenu();
  populateCameraMenu();
  populateModelMenu();
  populateToggleMenu();

  connect(ui->breakDurationSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationSpinBox_valueChanged(int)));
  connect(ui->breakDurationHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationHorizontalSlider_valueChanged(int)));
  connect(ui->breakIntervalSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalSpinBox_valueChanged(int)));
  connect(ui->breakIntervalHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalHorizontalSlider_valueChanged(int)));
  // Connect the FPS limit controls
  connect(ui->FPSLimitSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_FPSLimitSpinBox_valueChanged(int)));
  connect(ui->FPSLimitHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_FPSLimitHorizontalSlider_valueChanged(int)));

  // Show privacy information
  QMessageBox::information(
      this, "Privacy Information",
      "VisionGuard requires camera access to function properly."
      "The inference is done on your device and no data "
      "is sent externally. Your privacy is safe.");

  // Initialize the last frame time
  lastFrameTime = std::chrono::high_resolution_clock::now();
}

MainWindow::~MainWindow() {
  delete ui;
  delete timer;
  visionGuard.reset();
}

bool MainWindow::requestCameraPermission() {
  QCameraPermission cameraPermission;
  switch (qApp->checkPermission(cameraPermission)) {
  case Qt::PermissionStatus::Undetermined:
    qApp->requestPermission(cameraPermission, this,
                            &MainWindow::requestCameraPermission);
    return false;
  case Qt::PermissionStatus::Denied:
    return false;
  case Qt::PermissionStatus::Granted:
    return true;
  }
  return false;
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

void MainWindow::loadModels(const std::string &precision) {
  currentPrecision = precision;
  visionGuard.reset();
  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
}

void MainWindow::on_actionExit_triggered() { close(); }

void MainWindow::on_FPSLimitSpinBox_valueChanged(int value) {
  FPS_LIMIT = value;
  frameIntervalMs = 1000 / FPS_LIMIT;
  ui->FPSLimitHorizontalSlider->setValue(value);
}

void MainWindow::on_FPSLimitHorizontalSlider_valueChanged(int value) {
  FPS_LIMIT = value;
  frameIntervalMs = 1000 / FPS_LIMIT;
  ui->FPSLimitSpinBox->setValue(value);
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

std::vector<int> MainWindow::getAvailableCameras(int maxCameras) {
  std::vector<int> availableCameras;
  for (int i = 0; i < maxCameras; ++i) {
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
  return availableCameras;
}

QAction *MainWindow::createCameraAction(int cameraIndex) {
  QAction *cameraAction =
      new QAction(QString("Camera %1").arg(cameraIndex), this);
  cameraAction->setCheckable(true);
  if (cameraIndex == currentCameraIndex) {
    cameraAction->setChecked(true);
  }
  connect(cameraAction, &QAction::triggered, this, [this, cameraIndex]() {
    switchCamera(cameraIndex);
    updateCameraMenu(); // Update the menu to reflect the new state
  });
  return cameraAction;
}

void MainWindow::populateCameraMenu() {
  ui->menuCamera->clear();
  std::vector<int> availableCameras = getAvailableCameras(5);
  for (int cameraIndex : availableCameras) {
    QAction *cameraAction = createCameraAction(cameraIndex);
    ui->menuCamera->addAction(cameraAction);
  }
}

void MainWindow::updateCameraMenu() {
  for (QAction *action : ui->menuCamera->actions()) {
    bool isCurrent =
        action->text() == QString("Camera %1").arg(currentCameraIndex);
    action->setChecked(isCurrent);
  }
}

void MainWindow::populateDeviceMenu() {
  ui->menuDevices->clear();
  std::vector<std::string> availableDevices =
      visionGuard->getAvailableDevices();
  for (const auto &device : availableDevices) {
    QAction *deviceAction = new QAction(QString::fromStdString(device), this);
    deviceAction->setCheckable(true);
    if (device == currentDevice) {
      deviceAction->setChecked(true);
    }
    connect(deviceAction, &QAction::triggered, this, [this, device]() {
      switchDevice(device);
      updateDeviceMenu();
    });
    ui->menuDevices->addAction(deviceAction);
  }
}
void MainWindow::updateDeviceMenu() {
  for (QAction *action : ui->menuDevices->actions()) {
    action->setChecked(action->text().toStdString() == currentDevice);
  }
}
void MainWindow::populateModelMenu() {
  ui->menuModels->clear();
  std::vector<std::string> precisions = {INT8_PRECISION, FP16_PRECISION,
                                         FP32_PRECISION};
  for (const auto &precision : precisions) {
    QAction *precisionAction =
        new QAction(QString::fromStdString(precision), this);
    precisionAction->setCheckable(true);
    if (precision == currentPrecision) {
      precisionAction->setChecked(true);
    }
    connect(precisionAction, &QAction::triggered, this, [this, precision]() {
      loadModels(precision);
      updateModelMenu();
    });
    ui->menuModels->addAction(precisionAction);
  }
}

void MainWindow::updateModelMenu() {
  for (QAction *action : ui->menuModels->actions()) {
    action->setChecked(action->text().toStdString() == currentPrecision);
  }
}

void MainWindow::populateToggleMenu() {
  ui->menuToggles->clear();

  for (const auto &toggleAction : toggleActions) {
    QAction *action =
        new QAction(QString::fromStdString(toggleAction.name), this);
    action->setCheckable(true);
    connect(action, &QAction::triggered, this, [this, toggleAction]() {
      visionGuard->toggle(toggleAction.toggleType);
      updateToggleMenu(); // Update the menu to reflect the new state
    });
    ui->menuToggles->addAction(action);
  }

  updateToggleMenu(); // Initial update to set the correct check states
}

void MainWindow::updateToggleMenu() {
  // Update the check states of the toggle actions based on the current state
  for (QAction *action : ui->menuToggles->actions()) {
    if (action->text() == "Show All") {
      action->setChecked(visionGuard->isToggled(TOGGLE_ALL));
    } else if (action->text() == "Show Gaze") {
      action->setChecked(visionGuard->isToggled(TOGGLE_GAZE));
    } else if (action->text() == "Show Eye State") {
      action->setChecked(visionGuard->isToggled(TOGGLE_EYE_STATE));
    } else if (action->text() == "Show Landmarks") {
      action->setChecked(visionGuard->isToggled(TOGGLE_LANDMARKS));
    } else if (action->text() == "Show Head Pose Axes") {
      action->setChecked(visionGuard->isToggled(TOGGLE_HEAD_POSE_AXES));
    } else if (action->text() == "Show Face Bounding Box") {
      action->setChecked(visionGuard->isToggled(TOGGLE_FACE_BOUNDING_BOX));
    } else if (action->text() == "Show None") {
      action->setChecked(visionGuard->isToggled(TOGGLE_NONE));
    }
  }
}

void MainWindow::switchCamera(int cameraIndex) {
  currentCameraIndex = cameraIndex;

  // Close the existing capture device if it's open
  if (cap) {
    cap.reset();
  }

  // Open the new capture device
  cap = openImagesCapture(
      std::to_string(currentCameraIndex).c_str(), false, read_type::efficient,
      0, std::numeric_limits<size_t>::max(), stringToSize("1280x720"));

  if (cap) {
    slog::debug << "Capture device " << currentCameraIndex
                << " initialized successfully" << slog::endl;
  } else {
    slog::err << "Failed to initialize capture device " << currentCameraIndex
              << slog::endl;
  }

  updateCameraMenu(); // Update the menu to reflect the new state
}

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

void MainWindow::updateFrame() {
  auto now = std::chrono::high_resolution_clock::now();
  auto elapsedTime =
      std::chrono::duration_cast<std::chrono::milliseconds>(now - lastFrameTime)
          .count();

  if (elapsedTime < frameIntervalMs) {
    return;
  }

  lastFrameTime = now;

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
