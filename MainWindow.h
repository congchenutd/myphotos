#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);

private slots:
    void onScan();
    void onSettings();

private:
    Ui::MainWindow ui;
};

#endif // MAINWINDOW_H
