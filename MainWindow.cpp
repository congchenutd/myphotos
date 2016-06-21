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
#include "TagModel.h"
#include "PeopleModel.h"
#include "EventModel.h"
#include "SliderWithToolTip.h"

#include <QProgressBar>
#include <QActionGroup>
#include <QMessageBox>
#include <QDebug>
#include <QDir>

MainWindow* MainWindow::_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ascending(false)
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

    _slider = new SliderWithToolTip(this);
    _slider->setOrientation(Qt::Horizontal);
    _slider->setMinimumWidth(150);
    _slider->setMaximumWidth(150);
    _slider->setRange(100, 500);
    _slider->setValue(200);
    ui.statusBar->addPermanentWidget(_slider);
    ui.statusBar->addPermanentWidget(new QLabel("  "));

    connect(ui.actionAboutQt,       SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui.actionAbout,         SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui.actionScan,          SIGNAL(triggered(bool)),    SLOT(onScan()));
    connect(ui.actionOptions,       SIGNAL(triggered(bool)),    SLOT(onOptions()));
    connect(ui.actionSortByTitle,   SIGNAL(triggered()),        SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(triggered()),        SLOT(sort()));
    connect(ui.actionOrder,         SIGNAL(triggered()),        SLOT(onSortingOrder()));
    connect(ui.photoView, SIGNAL(selectionChanged(QList<PhotoItem*>)),
            this, SLOT(onPhotoSelected(QList<PhotoItem*>)));
    connect(ui.photoView, SIGNAL(newTag     (QString)), SLOT(onNewTag     (QString)));
    connect(ui.photoView, SIGNAL(newPeople  (QString)), SLOT(onNewPeople  (QString)));
    connect(ui.photoView, SIGNAL(newEvent   (QString, QDate)), SLOT(onNewEvent(QString, QDate)));
    connect(ui.actionRemove,    SIGNAL(triggered(bool)),    SLOT(onRemove()));
    connect(ui.actionDelete,    SIGNAL(triggered(bool)),    SLOT(onDelete()));
    connect(ui.actionRename,    SIGNAL(triggered(bool)),    SLOT(onRename()));
    connect(_slider,            SIGNAL(valueChanged(int)),  SLOT(onThumbnailSize(int)));

    ui.photoView->load(_library->getAllPhotos().values());
    sort();
    onPhotoSelected(QList<PhotoItem*>());

    ui.pageTags     ->setModel(new TagModel     (this));
    ui.pagePeople   ->setModel(new PeopleModel  (this));
    ui.pageEvents   ->setModel(new EventModel   (this));
    connect(ui.pageTags,    SIGNAL(filterByTags(QStringList, bool)),    SLOT(onFilterByTags     (QStringList, bool)));
    connect(ui.pagePeople,  SIGNAL(filterByTags(QStringList, bool)),    SLOT(onFilterByPeople   (QStringList, bool)));
    connect(ui.pageEvents,  SIGNAL(filter(QString)),                    SLOT(onFilterByEvent    (QString)));

    _scanner = new Scanner;
    connect(_scanner, SIGNAL(photoAdded(Photo*)), this, SLOT(onPhotoAdded(Photo*)));

    _slider->setValue(Settings::getInstance()->getThumbnailSize().width());
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
    ui.photoView->addPhoto(photo, _slider->value());
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

    ui.pageInfo->setCurrentPhoto(selected.isEmpty() ? 0 : selected.front()->getPhoto());
}

void MainWindow::onRename() {
    ui.photoView->getSelectedItems().front()->rename();
}

void MainWindow::onRemove()
{
    if (QMessageBox::warning(this, tr("Remove"),
                             tr("Are you sure to remove the selected photo(s) from the library, not physically from the computer?"),
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
                             tr("Are you sure to PHYSICALLY delete the selected photo(s) from the computer?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        for (PhotoItem* item: ui.photoView->getSelectedItems())
        {
            _library->removePhoto(item->getPhoto());
            ui.photoView->removeItem(item);

            QFileInfo fileInfo(item->getPhoto()->getFilePath());
            QString newPath = Settings::getInstance()->getTrashLocation() + QDir::separator() + fileInfo.fileName();
            QFile file(fileInfo.filePath());
            file.rename(newPath);
        }
    }
}

void MainWindow::onThumbnailSize(int size)
{
    ui.photoView->resizeThumbnails(size);
    Settings::getInstance()->setThumbnailSize(QSize(size, size));
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

    ui.pageTags->update();
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

    ui.pagePeople->update();
}

void MainWindow::onNewEvent(const QString& name, const QDate& date)
{
    Event* event = new Event(EventDAO::getInstance()->getNextID(), name, date);
    _library->addEvent(event);
    event->save();

    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->setEvent(event);
        photo->save();
    }

    ui.pageEvents->update();
}

void MainWindow::onFilterByTags(const QStringList& tags, bool AND)
{
    ui.photoView->clear();
    if (tags.isEmpty())
        resetPhotos();
    else
        ui.photoView->load(_library->filterPhotosByTags(tags.toSet(), AND));
    sort();
}

void MainWindow::onFilterByPeople(const QStringList& people, bool AND)
{
    ui.photoView->clear();
    if (people.isEmpty())
        resetPhotos();
    else
        ui.photoView->load(_library->filterPhotosByPeople(people.toSet(), AND));
    sort();
}

void MainWindow::onFilterByEvent(const QString& eventName)
{
    ui.photoView->clear();
    if (eventName.isEmpty())
        resetPhotos();
    else
        ui.photoView->load(_library->filterPhotosByEvent(eventName));
    sort();
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About",
        tr("<h3><b>MyPhotos: A Simple Photo Manager</b></h3>"
           "<p>Built on %1</p>"
           "<p>Cong Chen &lt;<a href=mailto:CongChenUTD@Gmail.com>CongChenUTD@Gmail.com</a>&gt;</p>")
                       .arg(Settings::getInstance()->getCompileDate()));
}

void MainWindow::resetPhotos() {
    ui.photoView->load(_library->getAllPhotos().values());
}
