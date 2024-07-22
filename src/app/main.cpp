#include "resources.qrc"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  QMainWindow w;
  Ui::MainWindow ui;
  ui.setupUi(&w);
  w.setWindowIcon(QIcon(
      "src/assets/openvino.png")); 

  // Connect theme actions
  QObject::connect(ui.actionLightTheme, &QAction::triggered, [&]() {
    qApp->setStyleSheet(""); // Clear stylesheet for light theme
  });

  QObject::connect(ui.actionDarkTheme, &QAction::triggered, [&]() {
    qApp->setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; color: #d3d3d3; }"
        "QLabel { color: #d3d3d3; }"
        "QPushButton { background-color: #444444; color: #d3d3d3; border: 1px "
        "solid #555555; }"
        "QPushButton:hover { background-color: #555555; }"
        "QSlider::groove:horizontal { height: 4px; background: #444444; }"
        "QSlider::handle:horizontal { background: #777777; width: 10px; }"
        "QTabWidget::pane { border-top: 1px solid #555555; }"
        "QTabBar::tab { background: #444444; color: #d3d3d3; padding: 10px; }"
        "QTabBar::tab:selected { background: #555555; }");
  });

  w.show();
  return a.exec();
}
