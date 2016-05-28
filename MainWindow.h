#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class Photo;
class QProgressBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

private slots:
    void onScan();
    void onSettings();
    void onPhotoAdded(Photo* photo);

private:
    Ui::MainWindow ui;
    QProgressBar*   _progressBar;
};

#endif // MAINWINDOW_H
