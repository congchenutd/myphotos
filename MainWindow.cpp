#include "MainWindow.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "Library.h"

#include <QProgressBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    _progressBar = new QProgressBar(this);
    _progressBar->setTextVisible(true);
    _progressBar->setFormat("%v/%m");
    _progressBar->hide();
    ui.statusBar->addPermanentWidget(_progressBar);

    connect(ui.actionScan,      SIGNAL(triggered(bool)), this, SLOT(onScan()));
    connect(ui.actionSettings,  SIGNAL(triggered(bool)), this, SLOT(onSettings()));
    connect(Library::getInstance(), SIGNAL(photoAdded(Photo*)), this, SLOT(onPhotoAdded(Photo*)));
}

void MainWindow::onScan()
{
    int nPhotos = Library::getInstance()->preScan();
    if (nPhotos > 0)
    {
        Library::getInstance()->scan();
        _progressBar->show();
        _progressBar->setRange(0, nPhotos);
        _progressBar->setValue(0);
    }
}

void MainWindow::onSettings()
{
    SettingsDlg dlg(this);
    dlg.exec();
}

void MainWindow::onPhotoAdded(Photo* photo)
{
    ui.photoView->addPhoto(photo);
    _progressBar->setValue(_progressBar->value() + 1);
    if (_progressBar->value() == _progressBar->maximum())
    {
        _progressBar->hide();
        ui.statusBar->showMessage(tr("%1 photo(s) imported").arg(_progressBar->maximum()), 2000);
    }
    else
        ui.statusBar->showMessage(tr("Importing %1 out of %2")
                                  .arg(_progressBar->value())
                                  .arg(_progressBar->maximum()));
    qApp->processEvents();
}
