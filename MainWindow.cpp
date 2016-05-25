#include "MainWindow.h"
#include "SettingsDlg.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.actionImport,    SIGNAL(triggered(bool)), this, SLOT(onImport()));
    connect(ui.actionSettings,  SIGNAL(triggered(bool)), this, SLOT(onSettings()));
}

void MainWindow::onImport()
{

}

void MainWindow::onSettings()
{
    SettingsDlg dlg(this);
    dlg.exec();
}
