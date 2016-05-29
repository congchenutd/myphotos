#include "MainWindow.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "Library.h"
#include "PhotoItem.h"

#include <QProgressBar>
#include <QActionGroup>
#include <QMessageBox>

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

    QSlider* slider = new QSlider(Qt::Horizontal, this);
    slider->setMinimumWidth(150);
    slider->setMaximumWidth(150);
    slider->setRange(100, 200);
    slider->setValue(200);
    ui.statusBar->addPermanentWidget(slider);
    ui.statusBar->addPermanentWidget(new QLabel("  "));

    connect(ui.actionScan,      SIGNAL(triggered(bool)), this, SLOT(onScan()));
    connect(ui.actionOptions,   SIGNAL(triggered(bool)), this, SLOT(onOptions()));
    connect(Library::getInstance(), SIGNAL(photoAdded(Photo*)), this, SLOT(onPhotoAdded(Photo*)));
    connect(ui.actionSortByTitle,   SIGNAL(triggered()), this, SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(triggered()), this, SLOT(sort()));
    connect(ui.actionOrder,         SIGNAL(triggered()), this, SLOT(onSortingOrder()));
    connect(ui.photoView, SIGNAL(selectionChanged(QList<PhotoItem*>)),
            this, SLOT(onPhotoSelected(QList<PhotoItem*>)));
    connect(ui.actionRemove, SIGNAL(triggered(bool)), this, SLOT(onRemove()));
    connect(ui.actionDelete, SIGNAL(triggered(bool)), this, SLOT(onDelete()));
    connect(ui.actionRename, SIGNAL(triggered(bool)), this, SLOT(onRename()));
    connect(slider, SIGNAL(valueChanged(int)), this, SLOT(onThumbnailSize(int)));

    sort();
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
    sort();
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

void MainWindow::onPhotoSelected(const QList<PhotoItem*>& selected)
{
    ui.actionRemove->setEnabled(!selected.isEmpty());
    ui.actionDelete->setEnabled(!selected.isEmpty());
    ui.actionRename->setEnabled(!selected.isEmpty());
}

void MainWindow::onRename() {
    ui.photoView->getSelectedItems().front()->rename();
}

void MainWindow::onRemove()
{
    if (QMessageBox::warning(this, tr("Remove"),
                             tr("Are you sure to remove the selected photo(s) from the library?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        for (PhotoItem* item: ui.photoView->getSelectedItems())
        {
            Library::getInstance()->removePhoto(item->getPhoto());
            ui.photoView->removeItem(item);
        }
    }
}

void MainWindow::onDelete()
{
    if (QMessageBox::warning(this, tr("Delete"),
                             tr("Are you sure to permanently delete the selected photo(s) from the computer?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {

    }
}

void MainWindow::onThumbnailSize(int size)
{
    ui.photoView->resizeThumbnails(size);
}
