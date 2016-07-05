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
#include "Settings.h"

#include <QFileSystemModel>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <algorithm>
#include <QAction>
#include <QDebug>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QShortcut>

PhotoView::PhotoView(QWidget *parent) :
    QWidget(parent),
    _rubberBand(0),
    _sortBy("Time"),
    _ascending(false)
{
    _library    = Library::getInstance();
    QVBoxLayout* layout = new QVBoxLayout(this);
    _vBoxLayout = new SortableVBoxLayout;
    layout->addLayout(_vBoxLayout);
    layout->addSpacing(1000);

    _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    _rubberBand->setGeometry(QRect(_clickedPosition, QSize()));

    new QShortcut(QKeySequence("Ctrl+A"), this, SLOT(onSelectAll()));
}

/**
 * Delete all the PhotoItems
 */
void PhotoView::clear()
{
    _vBoxLayout->clear();
    _selected.clear();
    _clusterViews.clear();
    _photoClusters.clear();
}

/**
 * Load photos into the view
 */
void PhotoView::load(const QList<Photo*>& photos)
{
    for (Photo* photo: photos)
        addPhoto(photo);
}

/**
 * Creates a PhotoItem and add it to a new or existing ClusterView
 */
void PhotoView::addPhoto(Photo* photo)
{
    // Get an exiting or new Cluster for the photo, and add the photo to the cluster
    Cluster* cluster = _photoClusters.addPhoto(photo);

    // new cluster, create a clusterview
    if (!_clusterViews.contains(cluster))
    {
        ClusterView* clusterView = new ClusterView(cluster, this);
        _clusterViews.insert(cluster, clusterView);
        _vBoxLayout->addWidget(clusterView);
    }
    // add to existing clusterview
    else
    {
        ClusterView* clusterView = _clusterViews[cluster];
        clusterView->addPhoto(photo);
    }
}

void PhotoView::sort(const QString& byWhat, bool ascending)
{
    _sortBy     = byWhat;
    _ascending  = ascending;

    // sort the clusterviews first, then the photoitems within each clusterview
    if (byWhat == "Address")
    {
        _vBoxLayout->sort(ClusterViewLessAddress(ascending));
        foreach (ClusterView* clusterView, _clusterViews)
            clusterView->sort(PhotoItemLessTime(false));
    }
    else if (byWhat == "Time")
    {
        _vBoxLayout->sort(ClusterViewLessDate(ascending));
        foreach (ClusterView* clusterView, _clusterViews)
            clusterView->sort(PhotoItemLessTime(ascending));
    }
    else if (byWhat == "Title")
    {
        _vBoxLayout->sort(ClusterViewLessDate(false));
        foreach (ClusterView* clusterView, _clusterViews)
            clusterView->sort(PhotoItemLessTitle(ascending));
    }
}

void PhotoView::sort() {
    sort(_sortBy, _ascending);
}

QList<PhotoItem*> PhotoView::getSelectedItems() const {
    return _selected.toList();
}

void PhotoView::removePhotoItem(PhotoItem* item)
{
    _selected.remove(item);
    Photo* photo = item->getPhoto();
    ClusterView* clusterView = item->getClusterView();
    clusterView->removePhotoItem(item);
    _photoClusters.removePhoto(photo);

    // when a clusterview becomes empty, remove the clusterview
    if (clusterView->getPhotoItemCount() == 0)
    {
        _vBoxLayout->removeWidget(clusterView);
        _clusterViews.remove(clusterView->getCluster());
        delete clusterView;
    }
}

/**
 * Resize the thumbnails based on Settings
 */
void PhotoView::resizeThumbnails() {
    foreach (PhotoItem* item, getAllPhotoItems())
        item->resizeThumbnail();
}

void PhotoView::mousePressEvent(QMouseEvent* event)
{
    static QPoint lastClickedPosition;

    // show the selection rubber band
    _clickedPosition = event->pos();
    _rubberBand->setGeometry(QRect(_clickedPosition, QSize()));
    _rubberBand->show();

    PhotoItem* clickedItem = getClickedItem(_clickedPosition);
    if (clickedItem == 0)
        _selected.clear();

    else if (event->button() == Qt::LeftButton)
    {
        // add clicked items to selection
        if (event->modifiers() == Qt::ShiftModifier)
            _selected += getClickedItems(lastClickedPosition, _clickedPosition);
        else if (event->modifiers() == Qt::ControlModifier)
            toggleSelection(clickedItem);
        else
            _selected = QSet<PhotoItem*>() << clickedItem;  // single selection
    }

    // show the selection
    updateSelection(_selected);
    lastClickedPosition = _clickedPosition;

    // context menu
    if (event->button() == Qt::RightButton)
    {
        // right clicked on an unselected item, reselect this item
        if (clickedItem != 0 && !_selected.contains(clickedItem))
        {
            _selected = QSet<PhotoItem*>() << clickedItem;
            updateSelection(_selected);
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

            menu.addSeparator();
            menu.addAction(createFavoriteAction());
        }
        menu.exec(event->globalPos());
    }
}

/**
 * Draw a selection rectangle and update selected photo items
 */
void PhotoView::mouseMoveEvent(QMouseEvent* event)
{
    QRect rubberBandRect(QRect(_clickedPosition, event->pos()).normalized());
    _rubberBand->setGeometry(rubberBandRect);

    _selected.clear();
    foreach (PhotoItem* item, getAllPhotoItems())
    {
        QRect itemRect = item->geometryMappedTo(this);
        if (rubberBandRect.contains  (itemRect) ||
            rubberBandRect.intersects(itemRect))
            _selected << item;
    }
    updateSelection(_selected);
}

void PhotoView::mouseReleaseEvent(QMouseEvent*) {
    _rubberBand->hide();
}

/**
 * Highlight selected items
 */
void PhotoView::updateSelection(const QSet<PhotoItem*>& selected)
{
    foreach (PhotoItem* item, getAllPhotoItems())
        item->setSelected(selected.contains(item));
    emit selectionChanged(selected.toList());
}

/**
 * @return  pointer to the photo item being clicked on
 */
PhotoItem* PhotoView::getClickedItem(const QPoint& point) const
{
    foreach (PhotoItem* item, getAllPhotoItems())
        if (item->geometryMappedTo(this).contains(point))
            return item;
    return 0;
}

/**
 * @return  index of the photo item being clicked on
 */
int PhotoView::getClickedItemIndex(const QPoint& point) const
{
    QList<PhotoItem*> items = getAllPhotoItems();
    for (int i = 0; i < items.count(); ++i)
        if (items.at(i)->geometryMappedTo(this).contains(point))
            return i;
    return -1;
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
    QList<PhotoItem*> items = getAllPhotoItems();
    for (int i = startIdx; i <= endIdx; ++i)
        result << items.at(i);
    return result;
}

QList<PhotoItem*> PhotoView::getAllPhotoItems() const
{
    QList<PhotoItem*> result;
    for (int i = 0; i < _vBoxLayout->count(); ++i)
        if (ClusterView* clusterView = dynamic_cast<ClusterView*>(_vBoxLayout->itemAt(i)->widget()))
            result << clusterView->getAllPhotoItems();
    return result;
}

PhotoItem* PhotoView::getItemAt(int index) const {
    return getAllPhotoItems().at(index);
}

/**
 * Mapping from Photo* to PhotoItem*
 */
PhotoItem* PhotoView::getItem(Photo* photo) const {
    foreach (PhotoItem* item, getAllPhotoItems())
        if (item->getPhoto() == photo)
            return item;
    return 0;
}

void PhotoView::showTitles(bool show) {
    foreach (PhotoItem* item, getAllPhotoItems())
        item->showTitle(show);
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
        if (ClusterView* clusterView = _clusterViews[cluster])
            clusterView->reloadTitle();
}

void PhotoView::onSelectAll()
{
    _selected = getAllPhotoItems().toSet();
    updateSelection(_selected);
}

void PhotoView::onSetFavorite(bool favorite)
{
    foreach (PhotoItem* item, getSelectedItems())
    {
        item->getPhoto()->setFavorite(favorite);
        item->getPhoto()->save();
    }
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

QAction* PhotoView::createFavoriteAction()
{
    QAction* action = new QAction(QIcon(":/Images/Heart.png"), tr("Favorite"));
    connect(action, SIGNAL(triggered(bool)), SLOT(onSetFavorite(bool)));
    action->setCheckable(true);
    action->setChecked(true);
    foreach (PhotoItem* item, _selected)
        if (!item->getPhoto()->isFavorite())
            action->setChecked(false);
    return action;
}
