#include "MainWindow.h"
#include "SettingsDlg.h"
#include "Settings.h"
#include "Library.h"
#include "PhotoItem.h"
#include "Tag.h"
#include "Photo.h"
#include "Scanner.h"
#include "TagDAO.h"
#include "PeopleDAO.h"
#include "Event.h"
#include "EventDAO.h"

#include <QProgressBar>
#include <QActionGroup>
#include <QMessageBox>
#include <QDebug>

MainWindow* MainWindow::_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ascending(true)
 {
    _instance = this;
    _library = Library::getInstance();
    _library->load();

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

    connect(ui.actionScan,          SIGNAL(triggered(bool)),    this, SLOT(onScan()));
    connect(ui.actionOptions,       SIGNAL(triggered(bool)),    this, SLOT(onOptions()));
    connect(ui.actionSortByTitle,   SIGNAL(triggered()),        this, SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(triggered()),        this, SLOT(sort()));
    connect(ui.actionOrder,         SIGNAL(triggered()),        this, SLOT(onSortingOrder()));
    connect(ui.photoView, SIGNAL(selectionChanged(QList<PhotoItem*>)),
            this, SLOT(onPhotoSelected(QList<PhotoItem*>)));
    connect(ui.photoView, SIGNAL(newTag     (QString)), this, SLOT(onNewTag     (QString)));
    connect(ui.photoView, SIGNAL(newPeople  (QString)), this, SLOT(onNewPeople  (QString)));
    connect(ui.photoView, SIGNAL(newEvent   (QString)), this, SLOT(onNewEvent   (QString)));
    connect(ui.actionRemove,    SIGNAL(triggered(bool)),    this, SLOT(onRemove()));
    connect(ui.actionDelete,    SIGNAL(triggered(bool)),    this, SLOT(onDelete()));
    connect(ui.actionRename,    SIGNAL(triggered(bool)),    this, SLOT(onRename()));
    connect(slider,             SIGNAL(valueChanged(int)),  this, SLOT(onThumbnailSize(int)));

    ui.photoView->load(_library->getAllPhotos().values());
    sort();
    onPhotoSelected(QList<PhotoItem*>());

    ui.pageTags->setTags(_library->getAllTags().keys());
    connect(ui.pageTags,    SIGNAL(filterByTags(QStringList, bool)),
            this,           SLOT(onFilterByTags(QStringList, bool)));

    ui.pagePeople->setTags(_library->getAllPeople().keys());
    connect(ui.pagePeople,  SIGNAL(filterByTags(QStringList, bool)),
            this,           SLOT(onFilterByPeople(QStringList, bool)));

    _scanner = new Scanner;
    connect(_scanner, SIGNAL(photoAdded(Photo*)), this, SLOT(onPhotoAdded(Photo*)));
}

MainWindow* MainWindow::getInstance()           { return _instance;             }
QAction*    MainWindow::getRenameAction()       { return ui.actionRename;       }
QAction*    MainWindow::getRemoveAction()       { return ui.actionRemove;       }
QAction*    MainWindow::getDeleteAction()       { return ui.actionDelete;       }
QAction*    MainWindow::getSortByTitleAction()  { return ui.actionSortByTitle;  }
QAction*    MainWindow::getSortByTimeAction()   { return ui.actionSortByTime;   }
QAction*    MainWindow::getSortingOrderAction() { return ui.actionOrder;        }

void MainWindow::closeEvent(QCloseEvent*) {
    _library->clean();
}

void MainWindow::onScan()
{
    if (int nPhotos = _scanner->scan())
    {
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
        sort();
    }
    else
        ui.statusBar->showMessage(tr("Importing %1 of %2")
                                  .arg(_progressBar->value())
                                  .arg(_progressBar->maximum()));
}

void MainWindow::onSortingOrder()
{
    _ascending = !_ascending;
    QString iconPath = _ascending ? ":/Images/Up.png" : ":/Images/Down.png";
    ui.actionOrder->setIcon(QIcon(iconPath));
    QString text = _ascending ? tr("Ascending") : tr("Descending");
    ui.actionOrder->setText(text);
    ui.actionOrder->setToolTip(text);
    sort();
}

void MainWindow::sort()
{
    QString sortBy = ui.actionSortByTitle->isChecked() ? "Title" : "Time";
    ui.photoView->sort(sortBy, _ascending);
}

void MainWindow::onPhotoSelected(const QList<PhotoItem*>& selected)
{
    bool hasSelection = !selected.isEmpty();
    ui.actionRemove ->setEnabled(hasSelection);
    ui.actionDelete ->setEnabled(hasSelection);
    ui.actionRename ->setEnabled(hasSelection);
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
            _library->removePhoto(item->getPhoto());
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

void MainWindow::onThumbnailSize(int size) {
    ui.photoView->resizeThumbnails(size);
}

void MainWindow::onFilterByTags(const QStringList& tags, bool AND)
{
    ui.photoView->clear();
    if (tags.isEmpty()) {
        ui.photoView->load(_library->getAllPhotos().values());
    }
    else
    {
        QList<Photo*> photos = _library->filterPhotosByTags(tags.toSet(), AND);
        ui.photoView->load(photos);
    }
    sort();
}

void MainWindow::onNewTag(const QString& tagValue)
{
    Tag* tag = new Tag(TagDAO::getInstance()->getNextID(), tagValue);
    _library->addTag(tag);
    tag->save();

    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->addTag(tag);
        photo->save();
    }

    ui.pageTags->setTags(_library->getAllTags().keys());
}

void MainWindow::onNewPeople(const QString& name)
{
    People* people = new People(PeopleDAO::getInstance()->getNextID(), name);
    _library->addPeople(people);
    people->save();

    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->addPeople(people);
        photo->save();
    }

    ui.pagePeople->setTags(_library->getAllPeople().keys());
}

void MainWindow::onNewEvent(const QString& name)
{
    Event* event = new Event(EventDAO::getInstance()->getNextID(), name, QDate::currentDate());
    _library->addEvent(event);
    event->save();

    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->setEvent(event);
        photo->save();
    }

//    ui.pagePeople->setTags(_library->getAllPeople().keys());
}

void MainWindow::onFilterByPeople(const QStringList& people, bool AND)
{
    ui.photoView->clear();
    if (people.isEmpty()) {
        ui.photoView->load(_library->getAllPhotos().values());
    }
    else
    {
        QList<Photo*> photos = _library->filterPhotosByPeople(people.toSet(), AND);
        ui.photoView->load(photos);
    }
    sort();
}
