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
#include "SearchLineEdit.h"
#include "RenameDlg.h"

#include <QProgressBar>
#include <QActionGroup>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QScrollBar>
#include <QFileDialog>

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

    // search edit
    _searchEdit = new SearchLineEdit(this);

    QLabel* label = new QLabel;
    label->setPixmap(QPixmap(":/Images/Search.png"));
    label->resize(16, 16);

    ClearButton* btClear = new ClearButton;
    btClear->setToolTip(tr("Clear"));
    btClear->setShortcut(QKeySequence(Qt::Key_Escape));
    btClear->resize(16, 16);

    _searchEdit->setLabel(label);
    _searchEdit->setClearButton (btClear);
    _searchEdit->setFocusShortcut(QKeySequence("Ctrl+F"));
    connect(_searchEdit, SIGNAL(filter(QString)), SLOT(onFilterbyTitle(QString)));

    ui.mainToolBar->addSeparator();
    ui.mainToolBar->addWidget(_searchEdit);
    ui.mainToolBar->addWidget(new QLabel("  ", this));

    connect(ui.actionAboutQt,       SIGNAL(triggered()), qApp,  SLOT(aboutQt()));
    connect(ui.actionAbout,         SIGNAL(triggered()),        SLOT(onAbout()));
    connect(ui.actionScan,          SIGNAL(triggered()),        SLOT(onScan()));
    connect(ui.actionOptions,       SIGNAL(triggered()),        SLOT(onOptions()));
    connect(ui.actionSortByAddress, SIGNAL(triggered(bool)),    SLOT(sort()));
    connect(ui.actionSortByTitle,   SIGNAL(triggered(bool)),    SLOT(sort()));
    connect(ui.actionSortByTime,    SIGNAL(triggered(bool)),    SLOT(sort()));
    connect(ui.actionOrder,         SIGNAL(triggered()),        SLOT(onSortingOrder()));
    connect(ui.photoView, SIGNAL(selectionChanged(QList<PhotoItem*>)),
            this, SLOT(onPhotoSelected(QList<PhotoItem*>)));
    connect(ui.photoView, SIGNAL(newTag     (QString)), SLOT(onNewTag     (QString)));
    connect(ui.photoView, SIGNAL(newPeople  (QString)), SLOT(onNewPeople  (QString)));
    connect(ui.photoView, SIGNAL(newEvent   (QString, QDate)), SLOT(onNewEvent(QString, QDate)));
    connect(ui.actionRemove,    SIGNAL(triggered()),    SLOT(onRemove()));
    connect(ui.actionDelete,    SIGNAL(triggered()),    SLOT(onDelete()));
    connect(ui.actionRename,    SIGNAL(triggered()),    SLOT(onRename()));
    connect(_slider,            SIGNAL(valueChanged(int)),  SLOT(onThumbnailSize(int)));
    connect(ui.pageInfo, SIGNAL(infoChanged(Photo*)), SLOT(onInfoChanged(Photo*)));
    connect(ui.actionShowPhotos,    SIGNAL(triggered(bool)), SLOT(onShowPhotos(bool)));
    connect(ui.actionShowVideos,    SIGNAL(triggered(bool)), SLOT(onShowVideos(bool)));
    connect(ui.actionShowFavorites, SIGNAL(triggered(bool)), SLOT(onShowFavorites(bool)));
    connect(ui.actionExport,        SIGNAL(triggered(bool)), SLOT(onExport()));

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
    if (dlg.exec() == QDialog::Accepted)
    {
        ui.photoView->updateTitleVisibility();
    }
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
    ui.scrollArea->verticalScrollBar()->setValue(0);    // scroll to top
}

void MainWindow::onPhotoSelected(const QList<PhotoItem*>& selected)
{
    bool hasSelection = !selected.isEmpty();
    ui.actionRemove ->setEnabled(hasSelection);
    ui.actionDelete ->setEnabled(hasSelection);
    ui.actionRename ->setEnabled(hasSelection);
    ui.actionExport ->setEnabled(hasSelection);

    ui.pageInfo->setCurrentPhoto(hasSelection ? selected.front()->getPhoto() : 0);

    if (hasSelection)   // show selection info
        ui.statusBar->showMessage(tr("%1 file(s) selected").arg(selected.length()));
    else                // show all info
        ui.statusBar->showMessage(tr("%1 photo(s), %2 video(s)")
                                  .arg(_library->getPhotoCount())
                                  .arg(_library->getVideoCount()));
}

/**
 * Trigger title editing
 */
void MainWindow::onRename()
{
    RenameDlg dlg(this);
    if (dlg.exec() == QDialog::Accepted)
    {
        QString title = dlg.getTitle();
        if (!title.isEmpty())
            ui.photoView->rename(title);
    }
}

void MainWindow::onRemove()
{
    if (QMessageBox::warning(this, tr("Remove"),
                             tr("Are you sure to remove the selected photo(s) from the library, not physically from the computer?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        for (PhotoItem* item: ui.photoView->getSelectedItems())
        {
            ui.photoView->removePhotoItem(item);
            _library->removePhoto(item->getPhoto());
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
    QList<PhotoItem*> selected = ui.photoView->getSelectedItems();
    for (int i = 0; i < selected.count(); ++ i)
    {
        Photo* photo = selected.at(i)->getPhoto();
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

void MainWindow::onFilterbyTitle(const QString& title)
{
    ui.photoView->clear();
    ui.photoView->load(_library->filterPhotosByTitle(title));
    sort();
}

void MainWindow::onShowPhotos(bool show)
{
    if (show)
    {
        ui.photoView->clear();
        ui.photoView->load(_library->getAllImages());
        sort();
        ui.actionShowVideos     ->setChecked(false);
        ui.actionShowFavorites  ->setChecked(false);
    }
    else
        onFilterbyTitle("");
}

void MainWindow::onShowVideos(bool show)
{
    if (show)
    {
        ui.photoView->clear();
        ui.photoView->load(_library->getAllVideos());
        sort();
        ui.actionShowPhotos     ->setChecked(false);
        ui.actionShowFavorites  ->setChecked(false);
    }
    else
        onFilterbyTitle("");
}

void MainWindow::onShowFavorites(bool show)
{
    if (show)
    {
        ui.photoView->clear();
        ui.photoView->load(_library->getFavorites());
        sort();
        ui.actionShowPhotos->setChecked(false);
        ui.actionShowVideos->setChecked(false);
    }
    else
        onFilterbyTitle("");
}

void MainWindow::onExport()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Select Export Directory"), ".",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (dir.isEmpty())
        return;

    QList<PhotoItem*> selectedItems = ui.photoView->getSelectedItems();
    int count = selectedItems.length();  // must > 0 if the action is enabled
    _progressBar->show();
    _progressBar->setRange(0, count);
    _progressBar->setValue(0);

    for (int i = 0; i < count; ++ i)
    {
        Photo* photo = selectedItems.at(i)->getPhoto();
        QString sourceFilePath = photo->getFilePath();
        QString destFilePath = dir + QDir::separator() + QFileInfo(sourceFilePath).fileName();
        QFile::copy(sourceFilePath, destFilePath);
        _progressBar->setValue(i+1);
        ui.statusBar->showMessage(tr("Exporting %1 of %2 file(s)")
                                  .arg(i+1)
                                  .arg(count));

        QApplication::instance()->processEvents();
    }
    _progressBar->hide();
    ui.statusBar->showMessage(tr("%1 file(s) exported to %2")
                              .arg(count)
                              .arg(dir));
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
    }
}
