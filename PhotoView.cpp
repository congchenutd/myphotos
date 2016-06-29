#include "PhotoView.h"
#include "FlowLayout.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoItem.h"
#include "MainWindow.h"
#include "NewItemMenu.h"
#include "Tag.h"
#include "TagDAO.h"
#include "Event.h"
#include "NewTagDlg.h"
#include "NewEventDlg.h"
#include "ClusterView.h"
#include "SortableVBoxLayout.h"

#include <QFileSystemModel>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <algorithm>
#include <QAction>
#include <QDebug>
#include <QSpacerItem>
#include <QVBoxLayout>

PhotoView::PhotoView(QWidget *parent) :
    QWidget(parent),
    _rubberBand(0),
    _sortBy("Time"),
    _ascending(false)
{
//    ui.setupUi(this);
//    _layout     = new FlowLayout(this);
    _library    = Library::getInstance();
    _vBoxLayout = new SortableVBoxLayout(this);
    setLayout(_vBoxLayout);
}

/**
 * Delete all the PhotoItems
 */
void PhotoView::clear()
{
//    _layout->clear();
    _selected.clear();
//    _photos.clear();

    while (QLayoutItem* item = _vBoxLayout->takeAt(0))
        item->widget()->deleteLater();
}

/**
 * Load photos into the view
 */
void PhotoView::load(const QList<Photo*>& photos)
{
    for (Photo* photo: photos)
        addPhoto(photo);
}

void PhotoView::addPhoto(Photo* photo)
{
    Cluster* cluster = _photoClusters.addPhoto(photo);
    if (!_cluster2ClusterView.contains(cluster))
    {
        ClusterView* clusterView = new ClusterView(cluster, this);
        _cluster2ClusterView.insert(cluster, clusterView);
        _vBoxLayout->addWidget(clusterView);
    }
    else
    {
        ClusterView* clusterView = _cluster2ClusterView[cluster];
        clusterView->addPhoto(photo);
    }
}

void PhotoView::sort(const QString& byWhat, bool ascending)
{
    _sortBy     = byWhat;
    _ascending  = ascending;

    if (byWhat == "Address")
        _vBoxLayout->sort(ClusterViewLessAddress(ascending));
    else if (byWhat == "Time") {
        _vBoxLayout->sort(ClusterViewLessDate(ascending));
        foreach (ClusterView* clusterView, _cluster2ClusterView)
            clusterView->sort(PhotoItemLessTime(ascending));
    }
    else if (byWhat == "Title")
    {
        _vBoxLayout->sort(ClusterViewLessAddress(ascending));
        foreach (ClusterView* clusterView, _cluster2ClusterView)
            clusterView->sort(PhotoItemLessTitle(ascending));
    }
}

void PhotoView::sort() {
    sort(_sortBy, _ascending);
}

QList<PhotoItem*> PhotoView::getSelectedItems() const {
    return _selected.toList();
}

void PhotoView::removeItem(PhotoItem* item)
{
//    _layout->removeWidget(item);
    item->deleteLater();
    _selected.remove(item);
//    _photos.remove(item->getPhoto());
}

// TODO: improve performance, maybe using multi-threading
void PhotoView::resizeThumbnails()
{
    foreach (PhotoItem* item, getAllPhotoItems())
        item->resizeThumbnail();
}

//PhotoItem* PhotoView::getItem(Photo* photo) const {
//    return _photos.contains(photo) ? _photos[photo] : 0;
//}

void PhotoView::mousePressEvent(QMouseEvent* event)
{
    static QPoint lastClickedPosition;

    // show the selection rubber band
    _clickedPosition = event->pos();
    if (!_rubberBand)
        _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    _rubberBand->setGeometry(QRect(_clickedPosition, QSize()));
    _rubberBand->show();

    PhotoItem* clickedItem = getClickedItem(_clickedPosition);
    if (clickedItem == 0)
        _selected.clear();

    else if (event->button() == Qt::LeftButton)
    {
        // add clicked item to selection
        if (event->modifiers() == Qt::ShiftModifier)
            _selected += getClickedItems(lastClickedPosition, _clickedPosition);
        else if (event->modifiers() == Qt::ControlModifier)
            toggleSelection(clickedItem);
        else
            _selected = QSet<PhotoItem*>() << clickedItem;
    }

    // show the selection
    updateSelection();
    lastClickedPosition = _clickedPosition;

    // context menu
    if (event->button() == Qt::RightButton)
    {
        if (clickedItem != 0 && !_selected.contains(clickedItem))
        {
            _selected = QSet<PhotoItem*>() << clickedItem;
            updateSelection();
        }

        QMenu menu(this);
        MainWindow* mainWindow = MainWindow::getInstance();
        menu.addAction(mainWindow->getSortByTitleAction());
        menu.addAction(mainWindow->getSortByTimeAction());
        menu.addAction(mainWindow->getSortingOrderAction());
        if (!_selected.isEmpty())
        {
            menu.addSeparator();
            menu.addAction(mainWindow->getRenameAction());
            menu.addAction(mainWindow->getRemoveAction());
            menu.addAction(mainWindow->getDeleteAction());

            menu.addSeparator();
            menu.addMenu(createEventMenu());
            menu.addMenu(createTagMenu());
            menu.addMenu(createPeopleMenu());
        }
        menu.exec(event->globalPos());
    }
}

/**
 * Draw a selection rectangle and add selected photo items to _selected
 */
void PhotoView::mouseMoveEvent(QMouseEvent* event)
{
    if (_rubberBand == 0)
        return;

    _rubberBand->setGeometry(QRect(_clickedPosition, event->pos()).normalized());
    QRect rubberBandRect = _rubberBand->geometry();

    _selected.clear();
    foreach (PhotoItem* item, getAllPhotoItems()) {
        if (rubberBandRect.contains  (item->geometry()) ||
            rubberBandRect.intersects(item->geometry()))
            _selected << item;
    }
    updateSelection();
}

void PhotoView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && _rubberBand != 0)
        _rubberBand->hide();
}

/**
 * Highlight selected items
 */
void PhotoView::updateSelection()
{
    foreach (PhotoItem* item, getAllPhotoItems())
        item->setSelected(_selected.contains(item));
    emit selectionChanged(_selected.toList());
}

/**
 * @return  index of the photo item being clicked on
 */
int PhotoView::getClickedItemIndex(const QPoint& point) const
{
//    for (int i = 0; i < _layout->count(); ++i)
//        if (PhotoItem* item = getItemAt(i))
//            if (item->geometry().contains(point))
//                return i;
    return -1;
}

/**
 * @return  pointer to the photo item being clicked on
 */
PhotoItem* PhotoView::getClickedItem(const QPoint& point) const
{
    int index = getClickedItemIndex(point);
    return index > -1 ? getItemAt(index) : 0;
}

/**
 * @return  a set of items between the 2 clicked positions
 */
QSet<PhotoItem*> PhotoView::getClickedItems(const QPoint& start, const QPoint& end) const
{
    int idx1 = getClickedItemIndex(start);
    int idx2 = getClickedItemIndex(end);
    int startIdx = qMax(0, qMin(idx1, idx2));
    int endIdx   = qMax(idx1, idx2);
    QSet<PhotoItem*> result;
    for (int i = startIdx; i <= endIdx; ++i)
        if (PhotoItem* item = getItemAt(i))
            result << item;
    return result;
}

/**
 * Toggle the selection of a photo item
 */
void PhotoView::toggleSelection(PhotoItem* clicked)
{
    if (_selected.contains(clicked))
        _selected.remove(clicked);
    else
        _selected << clicked;
}

// Toggle tag assignment
void PhotoView::onTagChecked(bool checked)
{
    QString tagValue = static_cast<QAction*>(sender())->text();
    foreach (PhotoItem* item, _selected)
    {
        Photo* photo = item->getPhoto();
        if (checked)
            photo->addTag(_library->getTag(tagValue));
        else
            photo->removeTag(tagValue);
        photo->save();
    }
}

// Toggle people assignment
void PhotoView::onPeopleChecked(bool checked)
{
    QString name = static_cast<QAction*>(sender())->text();
    foreach (PhotoItem* item, _selected)
    {
        Photo* photo = item->getPhoto();
        if (checked)
            photo->addPeople(_library->getPeople(name));
        else
            photo->removePeople(name);
        photo->save();
    }
}

// Toggle event assignment
void PhotoView::onEventChecked(bool checked)
{
    QString name = static_cast<QAction*>(sender())->text();
    foreach (PhotoItem* item, _selected)
    {
        Photo* photo = item->getPhoto();
        photo->setEvent(checked ? _library->getEvent(name) : 0);
        photo->save();
    }
}

void PhotoView::onLocationDecoded(Photo* photo)
{
    if (Cluster* cluster = photo->getCluster())
        if (ClusterView* clusterView = _cluster2ClusterView[cluster])
            clusterView->reloadTitle();
}

NewItemMenu* PhotoView::createTagMenu()
{
    NewItemMenu* tagMenu = new NewItemMenu(tr("New tag"), new NewTagDlg(tr("New Tag"), this), this);
    tagMenu->setIcon(QIcon(":/Images/Tag.png"));
    tagMenu->setTitle("Tags");
    connect(tagMenu, SIGNAL(newItemAdded(QString, QDate, QString)), this, SIGNAL(newTag(QString)));

    // Find tags common to all the selected photos
    QSet<QString> commonTags = _library->getAllTags().keys().toSet();
    foreach (PhotoItem* item, _selected)
        commonTags = commonTags.intersect(item->getPhoto()->getTagNames());

    // Check common tag menu items
    foreach (const QString& tag, _library->getAllTags().keys())
    {
        QAction* action = new QAction(tag, this);
        action->setCheckable(true);
        action->setChecked(commonTags.contains(tag));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(onTagChecked(bool)));
        tagMenu->addAction(action);
    }
    return tagMenu;
}

NewItemMenu* PhotoView::createPeopleMenu()
{
    NewItemMenu* peopleMenu = new NewItemMenu(tr("New people"), new NewTagDlg(tr("New People"), this), this);
    peopleMenu->setIcon(QIcon(":/Images/People.png"));
    peopleMenu->setTitle("People");
    connect(peopleMenu, SIGNAL(newItemAdded(QString, QDate, QString)), this, SIGNAL(newPeople(QString)));

    // Find people common to all selected photos
    QSet<QString> commonPeople = _library->getAllPeople().keys().toSet();
    foreach (PhotoItem* item, _selected)
        commonPeople = commonPeople.intersect(item->getPhoto()->getPeopleNames());

    // Check common people menu items
    foreach (const QString& name, _library->getAllPeople().keys())
    {
        QAction* action = new QAction(name, this);
        action->setCheckable(true);
        action->setChecked(commonPeople.contains(name));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(onPeopleChecked(bool)));
        peopleMenu->addAction(action);
    }
    return peopleMenu;
}

NewItemMenu* PhotoView::createEventMenu()
{
    NewItemMenu* eventMenu = new NewItemMenu(tr("New event"), new NewEventDlg(this), this);
    eventMenu->setIcon(QIcon(":/Images/Events.png"));
    eventMenu->setTitle("Events");
    connect(eventMenu, SIGNAL(newItemAdded(QString, QDate, QString)), this, SIGNAL(newEvent(QString, QDate)));

    // Find events common to all selected photos
    QSet<QString> commonEvents = _library->getAllEvents().keys().toSet();
    foreach (PhotoItem* item, _selected)
    {
        if (Event* event = item->getPhoto()->getEvent())
            commonEvents = commonEvents.intersect(QSet<QString>() << event->getName());
        else
        {
            commonEvents.clear();
            break;
        }
    }

    // Check common event menu items
    foreach (const QString& name, _library->getAllEvents().keys())
    {
        QAction* action = new QAction(name, this);
        action->setCheckable(true);
        action->setChecked(commonEvents.contains(name));
        connect(action, SIGNAL(triggered(bool)), this, SLOT(onEventChecked(bool)));
        eventMenu->addAction(action);
    }
    return eventMenu;
}

QList<PhotoItem*> PhotoView::getAllPhotoItems() const
{
    QList<PhotoItem*> result;
//    for (int i = 0; i < _layout->count(); ++i)
//        if (PhotoItem* item = getItemAt(i))
//            result << item;
    return result;
}

PhotoItem* PhotoView::getItemAt(int index) const {
//    return dynamic_cast<PhotoItem*>(_layout->itemAt(index)->widget());
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////////
ClusterViewLessAddress::ClusterViewLessAddress(bool lessThan) : _lessThan(lessThan) {}

bool ClusterViewLessAddress::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
    {
        bool result = view1->getTitle() < view2->getTitle();
        return _lessThan ? result : !result;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////
ClusterViewLessDate::ClusterViewLessDate(bool lessThan) : _lessThan(lessThan) {}

bool ClusterViewLessDate::operator()(QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
    {
        bool result = view1->getDate() < view2->getDate();
        return _lessThan ? result : !result;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
PhotoItemLessTime::PhotoItemLessTime(bool lessThan) : _lessThan(lessThan) {}

bool PhotoItemLessTime::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
    {
        bool result = item1->getPhoto()->getTimeTaken() < item2->getPhoto()->getTimeTaken();
        return _lessThan ? result : !result;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
PhotoItemLessTitle::PhotoItemLessTitle(bool lessThan) : _lessThan(lessThan) {}

bool PhotoItemLessTitle::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
    {
        bool result = item1->getPhoto()->getTitle() < item2->getPhoto()->getTitle();
        return _lessThan ? result : !result;
    }
    return false;
}
