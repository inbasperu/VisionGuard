#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>
#include <QDir>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPermissions>
#include <QProcess>
#include <QStandardPaths>
#include <QThread>
#include <QTime>
#include <QTimer>
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
#ifndef PATH_MAX
#define PATH_MAX MAX_PATH
#endif

bool MainWindow::quitting = false;
bool MainWindow::first_quit = true;

extern std::filesystem::path EXECUTABLE_PATH;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), currentDevice("AUTO"),
      currentPrecision("FP16"), currentCameraIndex(0) {
  ui->setupUi(this);
  initializeUI();

  if (requestCameraPermission()) {
    // If we need to wait for permission, don't initialize yet
    return;
  }

  initializeAfterPermission();
}

void MainWindow::initializeUI() {
  ui->breakDurationSpinBox->setValue(gazeDuration);
  ui->breakDurationHorizontalSlider->setValue(gazeDuration);
  ui->breakIntervalSpinBox->setValue(intervalDuration);
  ui->breakIntervalHorizontalSlider->setValue(intervalDuration);
  ui->FPSLimitSpinBox->setValue(FPS_LIMIT);
  ui->FPSLimitHorizontalSlider->setValue(FPS_LIMIT);

  connect(ui->breakDurationSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationSpinBox_valueChanged(int)));
  connect(ui->breakDurationHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakDurationHorizontalSlider_valueChanged(int)));
  connect(ui->breakIntervalSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalSpinBox_valueChanged(int)));
  connect(ui->breakIntervalHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_breakIntervalHorizontalSlider_valueChanged(int)));
  connect(ui->FPSLimitSpinBox, SIGNAL(valueChanged(int)), this,
          SLOT(on_FPSLimitSpinBox_valueChanged(int)));
  connect(ui->FPSLimitHorizontalSlider, SIGNAL(valueChanged(int)), this,
          SLOT(on_FPSLimitHorizontalSlider_valueChanged(int)));
}

bool MainWindow::requestCameraPermission() {
  QCameraPermission cameraPermission;
  switch (qApp->checkPermission(cameraPermission)) {
  case Qt::PermissionStatus::Granted:
    return false; // No need to wait, permission already granted
  case Qt::PermissionStatus::Denied:
    QMessageBox::warning(
        this, "Permission Denied",
        "Camera access is required. The application will now close.");
    QCoreApplication::quit();
    return true; // We're quitting, so return true to prevent further
                 // initialization
  case Qt::PermissionStatus::Undetermined:
    qApp->requestPermission(
        cameraPermission, [this](const QPermission &permission) {
          if (permission.status() == Qt::PermissionStatus::Granted) {
            QTimer::singleShot(0, this, &MainWindow::initializeAfterPermission);
          } else {
            QMessageBox::warning(
                this, "Permission Denied",
                "Camera access is required. The application will now close.");
            QCoreApplication::quit();
          }
        });
    return true; // We need to wait for the permission result
  }
  return false;
}

void MainWindow::initializeAfterPermission() {
  initializeVisionGuard();

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

  if (QSystemTrayIcon::isSystemTrayAvailable()) {
    iconPath =
        QString::fromStdString(getResourcePath("vision-guard-removebg.png"));
    createActions();
    createTrayIcon();
    setIcon();
    connect(trayIcon, &QSystemTrayIcon::activated, this,
            &MainWindow::iconActivated);
    trayIcon->show();
  } else {
    QMessageBox::warning(this, "System Tray",
                         "System tray is not available on this system. "
                         "The application will run without tray icon support.");
  }

  // Initialize the last frame time
  lastFrameTime = std::chrono::high_resolution_clock::now();

  // Show the main window
  show();
}

void MainWindow::initializeVisionGuard() {
  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
  if (!visionGuard) {
    QMessageBox::critical(
        this, "Error",
        "Failed to initialize VisionGuard. The application will now exit.");
    QCoreApplication::quit();
    return;
  }
  slog::debug << "VisionGuard backend initialized successfully" << slog::endl;
}

MainWindow::~MainWindow() {
  delete ui;
  delete timer;
  visionGuard.reset();
}

void MainWindow::createActions() {
  minimizeAction = new QAction(tr("Mi&nimize"), this);
  connect(minimizeAction, &QAction::triggered, this, &QWidget::hide);

  maximizeAction = new QAction(tr("Ma&ximize"), this);
  connect(maximizeAction, &QAction::triggered, this, &QWidget::showMaximized);

  restoreAction = new QAction(tr("&Restore"), this);
  connect(restoreAction, &QAction::triggered, this, &QWidget::showNormal);

  quitAction = new QAction(tr("&Quit"), this);
  quitAction->setShortcut(QKeySequence::Quit);
  connect(quitAction, &QAction::triggered, this, &MainWindow::quitApplication);
}

void MainWindow::quitApplication() {
  quitting = true;
  qApp->quit();
}

void MainWindow::createTrayIcon() {
  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setContextMenu(trayIconMenu);
}

void MainWindow::setIcon() {
  QIcon icon(iconPath);
  if (icon.isNull()) {
    qWarning() << "Failed to load icon from path:" << iconPath;
  } else {
    if (trayIcon) {
      trayIcon->setIcon(icon);
    }
    setWindowIcon(icon);
  }
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
  switch (reason) {
  // case QSystemTrayIcon::Trigger:
  case QSystemTrayIcon::DoubleClick:
    if (isVisible())
      hide();
    else
      show();
    break;
  default:
    break;
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (quitting || qApp->isSavingSession() ||
      (qApp->queryKeyboardModifiers() & Qt::ControlModifier)) {
    event->accept();
    qApp->quit();
  } else if (trayIcon && trayIcon->isVisible()) {
    hide();
    event->ignore();
    if (first_quit) {
      first_quit = false;
      trayIcon->showMessage(
          "VisionGuard",
          "The program will keep running in the system tray. To terminate the "
          "program, "
          "choose Quit in the context menu of the system tray entry.");
    }
  } else {
    QMainWindow::closeEvent(event);
  }
}

void MainWindow::restartApplication() {
  // Get the current application's file path and arguments
  QString program = QCoreApplication::applicationFilePath();
  QStringList arguments = QCoreApplication::arguments();
  arguments.removeFirst(); // Remove the program name from arguments

  // Close the current window
  this->close();

  // Start a new instance of the application
  QProcess::startDetached(program, arguments);

  // Quit the current instance
  QCoreApplication::quit();
}

void MainWindow::checkGazeTime() {
  if (visionGuard->checkGazeTimeExceeded()) {
    show();
    QApplication::beep();
    QString message =
        QString("You have been looking at the screen for more than %1 minutes. "
                "Please take a break for %2 seconds.")
            .arg(gazeDuration)
            .arg(intervalDuration);

    int reply = QMessageBox::question(this, "Break Alert", message,
                                      QMessageBox::Ok | QMessageBox::Cancel);
    if (reply == QMessageBox::Ok) {
      visionGuard->resetGazeTime();
    }
  }
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

std::string MainWindow::getResourcePath(const std::string &resourceName) {
  QString appPath = QApplication::applicationDirPath();
  QDir resourceDir(appPath);

#if defined(Q_OS_MAC)
  if (resourceDir.dirName() == "MacOS") {
    resourceDir.cdUp();
    resourceDir.cd("Resources");
  }
#elif defined(Q_OS_WIN)
  // On Windows, resources are typically in the same directory as the executable
#else
  // For Linux and other platforms, you might want to check for a "resources"
  // subdirectory
  if (resourceDir.exists("resources")) {
    resourceDir.cd("resources");
  }
#endif

  return resourceDir.filePath(QString::fromStdString(resourceName))
      .toStdString();
}

std::string MainWindow::getModelPath(const std::string &modelName,
                                     const std::string &precision) {
  std::string modelPath =
      getResourcePath(MODELS_DIR + "/" + modelName + "/" + precision + "/" +
                      modelName + MODEL_EXTENSION);

  if (!std::filesystem::exists(modelPath)) {
    slog::err << "Model file not found: " << modelPath << slog::endl;
    throw std::runtime_error("Model file not found: " + modelPath);
  }

  slog::debug << "Found " << modelName << " at " << modelPath << " with "
              << precision << " precision" << slog::endl;
  return modelPath;
}

std::unique_ptr<VisionGuard>
MainWindow::initializeVisionGuard(const std::string &precision,
                                  const std::string &device) {
  try {
    auto guard = std::make_unique<VisionGuard>(
        getModelPath(GAZE_MODEL_NAME, precision),
        getModelPath(FACE_MODEL_NAME, precision),
        getModelPath(HEAD_POSE_MODEL_NAME, precision),
        getModelPath(LANDMARKS_MODEL_NAME, precision),
        getModelPath(EYE_STATE_MODEL_NAME, precision), device);
    guard->defaultCalibration(this->imageSize);
    return guard;
  } catch (const std::exception &e) {
    QMessageBox::critical(
        this, "Error",
        QString("Failed to initialize VisionGuard: %1").arg(e.what()));
    return nullptr;
  }
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Escape) {
    hide();
  } else {
    visionGuard->toggle(event->key());
  }
}

void MainWindow::loadModels(const std::string &precision) {
  currentPrecision = precision;
  visionGuard.reset();
  visionGuard = initializeVisionGuard(currentPrecision, currentDevice);
}

void MainWindow::on_actionExit_triggered() {
  quitting = true;
  close();
}
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
  gazeDuration = value;
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(value));
  ui->breakDurationSpinBox->setValue(value);
}

void MainWindow::on_breakDurationSpinBox_valueChanged(int arg1) {
  gazeDuration = arg1;
  visionGuard->setAccumulatedGazeTimeThreshold(static_cast<double>(arg1));
  ui->breakDurationHorizontalSlider->setValue(arg1);
}

void MainWindow::on_breakIntervalHorizontalSlider_valueChanged(int value) {
  intervalDuration = value;
  visionGuard->setGazeLostThreshold(static_cast<double>(value));
  ui->breakIntervalSpinBox->setValue(value);
}

void MainWindow::on_breakIntervalSpinBox_valueChanged(int arg1) {
  intervalDuration = arg1;
  visionGuard->setGazeLostThreshold(static_cast<double>(arg1));
  ui->breakIntervalHorizontalSlider->setValue(arg1);
}

void MainWindow::on_Calibrate_clicked() { this->performFourPointCalibration(); }

void MainWindow::performFourPointCalibration() {
  QScreen *screen = QGuiApplication::primaryScreen();
  QRect screenGeometry = screen->geometry();
  int screenWidth = screenGeometry.width();
  int screenHeight = screenGeometry.height();

  class CalibrationDialog : public QDialog {
  public:
    CalibrationDialog(QWidget *parent = nullptr) : QDialog(parent) {
      setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
      setModal(true);
      setStyleSheet("background-color: black;");
    }

  protected:
    void keyPressEvent(QKeyEvent *event) override {
      if (event->key() == Qt::Key_Space) {
        accept();
      }
    }
  };

  CalibrationDialog calibrationWindow(this);
  calibrationWindow.setGeometry(screenGeometry);

  QLabel instructionLabel(&calibrationWindow);
  instructionLabel.setAlignment(Qt::AlignCenter);
  instructionLabel.setStyleSheet("color: white; font-size: 24px;");
  instructionLabel.setGeometry(0, 0, screenWidth, screenHeight);

  int margin = 50;
  int pointSize = 20;
  std::vector<QPoint> calibrationPoints = {
      QPoint(margin, margin + 50), QPoint(screenWidth - margin, margin + 50),
      QPoint(screenWidth - margin, screenHeight - margin),
      QPoint(margin, screenHeight - margin)};

  std::vector<QString> pointNames = {"Top Left", "Top Right", "Bottom Right",
                                     "Bottom Left"};
  std::vector<cv::Point2f> screenBoundary;

  for (size_t i = 0; i < calibrationPoints.size(); ++i) {
    QLabel pointLabel(&calibrationWindow);
    pointLabel.setGeometry(calibrationPoints[i].x() - pointSize / 2,
                           calibrationPoints[i].y() - pointSize / 2, pointSize,
                           pointSize);
    pointLabel.setStyleSheet("background-color: #00FF00; border-radius: 10px;");
    pointLabel.show();

    instructionLabel.setText("Look at the " + pointNames[i] + " point");
    calibrationWindow.show();

    // Give the user a moment to focus on the point
    QThread::msleep(1500);

    cv::Point2f averageGazePoint;
    try {
      slog::debug << "Capturing " << pointNames[i].toStdString() << slog::endl;
      averageGazePoint = captureAverageGazePoint();
    } catch (const std::exception &e) {
      QMessageBox::critical(
          this, "Calibration Error",
          QString("Error during gaze capture: %1").arg(e.what()));
      return;
    }

    screenBoundary.push_back(averageGazePoint);
    pointLabel.hide();
  }

  calibrationWindow.close();

  try {
    visionGuard->setCalibrationPoints(screenBoundary[0], screenBoundary[1],
                                      screenBoundary[2], screenBoundary[3]);
  } catch (const std::exception &e) {
    QMessageBox::critical(
        this, "Calibration Error",
        QString("Error setting calibration points: %1").arg(e.what()));
    return;
  }

  QMessageBox::information(
      this, "Calibration Complete",
      "The 4-point calibration has been completed successfully.");
}

cv::Point2f MainWindow::captureAverageGazePoint() {
  int durationMs = 2500;
  std::vector<cv::Point2f> gazePoints;
  QElapsedTimer timer;
  timer.start();

  while (timer.elapsed() < durationMs) {
    cv::Mat frame = cap->read();
    if (frame.empty()) {
      throw std::runtime_error("Failed to capture frame from camera");
    }

    try {
      cv::Point2f gazePoint = visionGuard->captureGazePoint(frame);
      gazePoints.push_back(gazePoint);
    } catch (const std::exception &e) {
      throw std::runtime_error(std::string("Error capturing gaze point: ") +
                               e.what());
    }

    QCoreApplication::processEvents();
  }

  if (gazePoints.empty()) {
    throw std::runtime_error("No gaze points captured");
  }

  // Calculate average gaze point
  cv::Point2f averagePoint(0, 0);
  for (const auto &point : gazePoints) {
    averagePoint += point;
  }
  averagePoint *= 1.0f / gazePoints.size();

  slog::debug << "Average point value: " << averagePoint << " (from "
              << gazePoints.size() << " samples)" << slog::endl;

  return averagePoint;
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
      0, std::numeric_limits<size_t>::max(), stringToSize("600x400"));

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
