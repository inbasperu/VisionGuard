#include "constants.hpp"
#include "resources.qrc"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;
  Ui::MainWindow ui;
  ui.setupUi(&w);

  // Set window icon
  w.setWindowIcon(QIcon(WINDOW_ICON_PATH));

  // Connect theme actions
  QObject::connect(ui.actionLightTheme, &QAction::triggered, [&]() {
    qApp->setStyleSheet(LIGHT_THEME_STYLESHEET); // Apply light theme stylesheet
  });

  QObject::connect(ui.actionDarkTheme, &QAction::triggered, [&]() {
    qApp->setStyleSheet(DARK_THEME_STYLESHEET); // Apply dark theme stylesheet
  });

  // Show the main window
  w.show();
  return a.exec();
}
