#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionShow_None_triggered();

    void on_actionSettings_triggered();

    void on_actionShow_Landmarks_triggered();

    void on_actionShow_Head_Pose_Axes_triggered();

    void on_actionShow_Gaze_triggered();

    void on_actionShow_Face_Bounding_Box_triggered();

    void on_actionShow_Eye_State_triggered();

    void on_actionShow_All_triggered();

    void on_Calibrate_clicked();

    void showGazeTime();

    void showGazeLostTime();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
