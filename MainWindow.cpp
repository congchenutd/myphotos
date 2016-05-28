#include "MainWindow.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "Library.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    connect(ui.actionScan,      SIGNAL(triggered(bool)), this, SLOT(onScan()));
    connect(ui.actionSettings,  SIGNAL(triggered(bool)), this, SLOT(onSettings()));
    connect(Library::getInstance(), SIGNAL(photoAdded(Photo*)), ui.photoView, SLOT(addPhoto(Photo*)));
}

void MainWindow::onScan()
{
    Library::getInstance()->scan();
}

void MainWindow::onSettings()
{
    SettingsDlg dlg(this);
    dlg.exec();
}
