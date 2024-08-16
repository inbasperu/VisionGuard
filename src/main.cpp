#include "mainwindow.h"
#include <QApplication>
#include <filesystem>
#include <iostream>

std::filesystem::path EXECUTABLE_PATH;

int main(int argc, char *argv[]) {

  EXECUTABLE_PATH = std::filesystem::path(argv[0]).parent_path();

  std::cout << "Starting VisionGuard App" << std::endl;
  std::cout << "Executable path: " << EXECUTABLE_PATH.string() << std::endl;

  QApplication a(argc, argv);

  MainWindow w;
  w.show();

  return a.exec();
}
