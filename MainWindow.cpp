#include "MainWindow.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "Library.h"

#include <QProgressBar>
#include <QActionGroup>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ascending(true)
{
    ui.setupUi(this);
    _progressBar = new QProgressBar(this);
    _progressBar->setTextVisible(true);
    _progressBar->setFormat("%v/%m");
    _progressBar->hide();
    ui.statusBar->addPermanentWidget(_progressBar);

    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui.actionSortByTitle);
    actionGroup->addAction(ui.actionSortByTime);
    ui.actionSortByTime->setChecked(true);

    connect(ui.actionScan,      SIGNAL(triggered(bool)), this, SLOT(onScan()));
    connect(ui.actionOptions,   SIGNAL(triggered(bool)), this, SLOT(onOptions()));
    connect(Library::getInstance(), SIGNAL(photoAdded(Photo*)), this, SLOT(onPhotoAdded(Photo*)));
    connect(ui.photoView, SIGNAL(photoSelected(Photo*)), ui.exifView, SLOT(onPhotoSelected(Photo*)));
    connect(ui.actionSortByTitle,   SIGNAL(triggered()), this, SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(triggered()), this, SLOT(sort()));
    connect(ui.actionOrder,         SIGNAL(triggered()), this, SLOT(onSortingOrder()));
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

void MainWindow::onOptions()
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

void MainWindow::onSortingOrder()
{
    _ascending = !_ascending;
    QString iconPath = _ascending ? ":/Images/Up.png" : ":/Images/Down.png";
    ui.actionOrder->setIcon(QIcon(iconPath));
    sort();
}

void MainWindow::sort()
{
    QString sortBy = ui.actionSortByTitle->isChecked() ? "Title" : "Time";
    ui.photoView->sort(sortBy, _ascending);
}
