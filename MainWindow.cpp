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
#include "Geocoder.h"

#include <QProgressBar>
#include <QActionGroup>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QScrollBar>

MainWindow* MainWindow::_instance = 0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    _ascending(false)
 {
    _instance = this;
    _library = Library::getInstance();
    _library->load();

    ui.setupUi(this);

    // progress bar and status bar
    _progressBar = new QProgressBar(this);
    _progressBar->setTextVisible(true);
    _progressBar->setFormat("%v/%m");
    _progressBar->hide();
    ui.statusBar->addPermanentWidget(_progressBar);

    // action group makes the actions exclusive
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui.actionSortByTime);
    actionGroup->addAction(ui.actionSortByAddress);
    actionGroup->addAction(ui.actionSortByTitle);
    ui.actionSortByTime->setChecked(true);

    // slider shows the size of thumbnails
    _slider = new SliderWithToolTip(this);
    _slider->setOrientation(Qt::Horizontal);
    _slider->setMinimumWidth(150);
    _slider->setMaximumWidth(150);
    _slider->setRange(100, 500);
    _slider->setValue(200);
    _slider->setValue(Settings::getInstance()->getThumbnailSize().width());
    ui.statusBar->addPermanentWidget(_slider);
    ui.statusBar->addPermanentWidget(new QLabel("  "));

    connect(ui.actionAboutQt,       SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui.actionAbout,         SIGNAL(triggered()), this, SLOT(onAbout()));
    connect(ui.actionScan,          SIGNAL(triggered(bool)),    SLOT(onScan()));
    connect(ui.actionOptions,       SIGNAL(triggered(bool)),    SLOT(onOptions()));
    connect(ui.actionSortByAddress, SIGNAL(toggled(bool)),        SLOT(sort()));
    connect(ui.actionSortByTitle,   SIGNAL(toggled(bool)),        SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(toggled(bool)),        SLOT(sort()));
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
    connect(ui.pageInfo, SIGNAL(infoChanged(Photo*)), SLOT(onInfoChanged(Photo*)));

    // load photos to photo view
    ui.photoView->load(_library->getAllPhotos().values());
    onPhotoSelected(QList<PhotoItem*>());   // clear selection and disable actions

    // filtering pages
    ui.pageTags     ->setModel(new TagModel     (this));
    ui.pagePeople   ->setModel(new PeopleModel  (this));
    ui.pageEvents   ->setModel(new EventModel   (this));
    connect(ui.pageTags,    SIGNAL(filter(QStringList, bool)),  SLOT(onFilterByTags     (QStringList, bool)));
    connect(ui.pagePeople,  SIGNAL(filter(QStringList, bool)),  SLOT(onFilterByPeople   (QStringList, bool)));
    connect(ui.pageEvents,  SIGNAL(filter(QString)),            SLOT(onFilterByEvent    (QString)));

    _scanner = new Scanner;
    connect(_scanner, SIGNAL(scanned(Photo*)), SLOT(onPhotoScanned(Photo*)));

    _geocoder = new Geocoder;
    connect(_geocoder, SIGNAL(decoded(Photo*)), ui.photoView, SLOT(onLocationDecoded(Photo*)));
    _geocoder->start(_library->getAllPhotos().values());

    sort();
}

MainWindow* MainWindow::getInstance()           { return _instance;             }
QAction*    MainWindow::getRenameAction()       { return ui.actionRename;       }
QAction*    MainWindow::getRemoveAction()       { return ui.actionRemove;       }
QAction*    MainWindow::getDeleteAction()       { return ui.actionDelete;       }
QAction*    MainWindow::getSortByTitleAction()  { return ui.actionSortByTitle;  }
QAction*    MainWindow::getSortByTimeAction()   { return ui.actionSortByTime;   }
QAction*    MainWindow::getSortingOrderAction() { return ui.actionOrder;        }

void MainWindow::closeEvent(QCloseEvent* event) {
    _library->clean();  // clean up unused tags and save the library
    QMainWindow::closeEvent(event);
}

void MainWindow::onScan()
{
    _newPhotos = _scanner->scan();
    if (_newPhotos.length() > 0)
    {
        _progressBar->show();
        _progressBar->setRange(0, _newPhotos.length());
        _progressBar->setValue(0);
    }
}

void MainWindow::onOptions()
{
    SettingsDlg dlg(this);
    dlg.exec();
}

void MainWindow::onPhotoScanned(Photo* photo)
{
    ui.photoView->addPhoto(photo);

    _progressBar->setValue(_progressBar->value() + 1);
    sort();

    // all scanned
    if (_progressBar->value() == _progressBar->maximum())
    {
        _progressBar->hide();
        ui.statusBar->showMessage(tr("%1 photo(s) imported").arg(_progressBar->maximum()), 2000);
        _geocoder->start(_newPhotos);
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
    QString sortBy = "Time";
    if (ui.actionSortByAddress->isChecked())
        sortBy = "Address";
    else if (ui.actionSortByTitle->isChecked())
        sortBy = "Title";
    ui.photoView->sort(sortBy, _ascending);
}

void MainWindow::onPhotoSelected(const QList<PhotoItem*>& selected)
{
    bool hasSelection = !selected.isEmpty();
    ui.actionRemove ->setEnabled(hasSelection);
    ui.actionDelete ->setEnabled(hasSelection);
    ui.actionRename ->setEnabled(hasSelection);

    ui.pageInfo->setCurrentPhoto(hasSelection ? selected.front()->getPhoto() : 0);

    if (hasSelection)   // show selection info
        ui.statusBar->showMessage(tr("%1 photo(s) selected").arg(selected.length()));
    else                // show all info
        ui.statusBar->showMessage(tr("%1 photo(s), %2 video(s)")
                                  .arg(_library->getPhotoCount())
                                  .arg(_library->getVideoCount()));
}

/**
 * Trigger title editing
 */
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
            ui.photoView->removePhotoItem(item);
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
            ui.photoView->removePhotoItem(item);

            // move the file to trash
            QFileInfo fileInfo(item->getPhoto()->getFilePath());
            QString newPath = Settings::getInstance()->getTrashLocation() + QDir::separator() + fileInfo.fileName();
            QFile(fileInfo.filePath()).rename(newPath);
        }
    }
}

void MainWindow::onThumbnailSize(int size)
{
    Settings::getInstance()->setThumbnailSize(QSize(size, size));
    ui.photoView->resizeThumbnails();
}

void MainWindow::onNewTag(const QString& tagValue)
{
    // create a tag object
    Tag* tag = new Tag(TagDAO::getInstance()->getNextID(), tagValue);
    _library->addTag(tag);
    tag->save();

    // add the tag to selected photos
    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->addTag(tag);
        photo->save();
    }

    // update tag page
    ui.pageTags->update();
}

void MainWindow::onNewPeople(const QString& name)
{
    // create a people object
    People* people = new People(PeopleDAO::getInstance()->getNextID(), name);
    _library->addPeople(people);
    people->save();

    // add the people to selected photos
    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->addPeople(people);
        photo->save();
    }

    // update people page
    ui.pagePeople->update();
}

void MainWindow::onNewEvent(const QString& name, const QDate& date)
{
    // create an event object
    Event* event = new Event(EventDAO::getInstance()->getNextID(), name, date);
    _library->addEvent(event);
    event->save();

    // add it to selected photos
    foreach(PhotoItem* item, ui.photoView->getSelectedItems())
    {
        Photo* photo = item->getPhoto();
        photo->setEvent(event);
        photo->save();
    }

    // update event page
    ui.pageEvents->update();
}

void MainWindow::onFilterByTags(const QStringList& tags, bool AND)
{
    ui.photoView->clear();
    ui.photoView->load(_library->filterPhotosByTags(tags.toSet(), AND));
    sort();
}

void MainWindow::onFilterByPeople(const QStringList& people, bool AND)
{
    ui.photoView->clear();
    ui.photoView->load(_library->filterPhotosByPeople(people.toSet(), AND));
    sort();
}

void MainWindow::onFilterByEvent(const QString& eventName)
{
    ui.photoView->clear();
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

/**
 * Information page changed the meta data of a given photo
 * Update the corresponding PhotoItem
 */
void MainWindow::onInfoChanged(Photo* photo)
{
    if (PhotoItem* item = ui.photoView->getItem(photo))
    {
        item->setPhoto(photo);
        sort();
    }
}
