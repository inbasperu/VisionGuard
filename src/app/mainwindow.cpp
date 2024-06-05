#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{

}

void MainWindow::on_actionSettings_triggered()
{

}


void MainWindow::on_actionShow_Landmarks_triggered()
{

}


void MainWindow::on_actionShow_Head_Pose_Axes_triggered()
{

}


void MainWindow::on_actionShow_Gaze_triggered()
{

}


void MainWindow::on_actionShow_Face_Bounding_Box_triggered()
{

}


void MainWindow::on_actionShow_Eye_State_triggered()
{

}


void MainWindow::on_actionShow_All_triggered()
{

}


void MainWindow::on_actionShow_None_triggered()
{

}


void MainWindow::on_Calibrate_clicked()
{

}


void MainWindow::showGazeTime()
{

}


void MainWindow::showGazeLostTime()
{

}

