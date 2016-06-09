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

#include <QFileSystemModel>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <algorithm>
#include <QAction>
#include <QDebug>

PhotoView::PhotoView(QWidget *parent) :
    QWidget(parent),
    _rubberBand(0),
    _sortBy("Time"),
    _ascending(true)
{
    ui.setupUi(this);
    _layout = new FlowLayout(this);
    _library = Library::getInstance();
}

void PhotoView::load(const QList<Photo*>& photos)
{
    for (Photo* photo: photos)
        addPhoto(photo);
}

void PhotoView::sort(const QString& byWhat, bool ascending)
{
    _sortBy     = byWhat;
    _ascending  = ascending;

    if (byWhat == "Title") {
        if (ascending)  _layout->sort(PhotoItemLessTitle());
        else            _layout->sort(PhotoItemGreaterTitle());
    }
    if (byWhat == "Time") {
        if (ascending)  _layout->sort(PhotoItemLessTime());
        else            _layout->sort(PhotoItemGreaterTime());
    }
}

void PhotoView::sort() {
    sort(_sortBy, _ascending);
}

QList<PhotoItem*> PhotoView::getSelectedItems() const {
    return _selected;
}

void PhotoView::removeItem(PhotoItem* item) {
    _layout->removeWidget(item);
    item->deleteLater();
}

void PhotoView::resizeThumbnails(int size)
{
    for (int i = 0; i < _layout->count(); ++i)
    {
        PhotoItem* item = (PhotoItem*) _layout->itemAt(i)->widget();
        item->resizeThumbnail(size);
    }
}

void PhotoView::addPhoto(Photo* photo)
{
    PhotoItem* photoItem = new PhotoItem(photo);
    connect(photoItem, SIGNAL(titleEdited(QString)), this, SLOT(sort()));
    _layout->addWidget(photoItem);
}

void PhotoView::mousePressEvent(QMouseEvent* event)
{
    // show the selection rubber band
    _selectionStart = event->pos();
    if (!_rubberBand)
        _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    _rubberBand->setGeometry(QRect(_selectionStart, QSize()));
    _rubberBand->show();

    // single click will clear the selection
    if (event->button() == Qt::LeftButton && event->modifiers() != Qt::ShiftModifier)
        _selected.clear();

    // add clicked item to selection
    PhotoItem* clickedItem = getClickedItem(_selectionStart);
    if (clickedItem != 0)
        _selected << clickedItem;

    // show the selection
    updateSelection();

    // context menu
    if (event->button() == Qt::RightButton)
    {
        // right click on empty space will clear the selection
        if (clickedItem == 0)
        {
            _selected.clear();
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

            menu.addMenu(createTagMenu());
            menu.addMenu(createPeopleMenu());
        }
        menu.exec(event->globalPos());
    }
}

void PhotoView::onNewTag(const QString& tagValue)
{
    Tag* tag = new Tag(TagDAO::getInstance()->getNextID(), tagValue);
    _library->addTag(tag);
    tag->save();
    foreach(PhotoItem* item, _selected)
    {
        Photo* photo = item->getPhoto();
        photo->addTag(tag);
        photo->save();
    }
}

void PhotoView::onTagChecked(bool checked)
{
    QString tagValue = ((QAction*) sender())->text();
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

void PhotoView::onPeopleChecked(bool checked)
{
    QString name = ((QAction*) sender())->text();
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

NewItemMenu* PhotoView::createTagMenu()
{
    NewItemMenu* tagMenu = new NewItemMenu(tr("New tag"), this);
    tagMenu->setIcon(QIcon(":/Images/Tag.png"));
    tagMenu->setTitle("Tags");
    connect(tagMenu, SIGNAL(newItemAdded(QString)), this, SIGNAL(newTag(QString)));

    QSet<QString> commonTags = _library->getAllTags().keys().toSet();
    foreach (PhotoItem* item, _selected)
        commonTags = commonTags.intersect(item->getPhoto()->getTagValues());

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
    NewItemMenu* peopleMenu = new NewItemMenu(tr("New people"), this);
    peopleMenu->setIcon(QIcon(":/Images/People.png"));
    peopleMenu->setTitle("People");
    connect(peopleMenu, SIGNAL(newItemAdded(QString)), this, SIGNAL(newPeople(QString)));

    QSet<QString> commonPeople = _library->getAllPeople().keys().toSet();
    foreach (PhotoItem* item, _selected)
        commonPeople = commonPeople.intersect(item->getPhoto()->getPeopleNames());

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

void PhotoView::clear() {
    _layout->clear();
}

void PhotoView::mouseMoveEvent(QMouseEvent* event)
{
    _rubberBand->setGeometry(QRect(_selectionStart, event->pos()).normalized());
    QRect rubberBandRect = _rubberBand->geometry();

    _selected.clear();
    for (int i = 0; i < _layout->count(); ++i)
    {
        PhotoItem* item = (PhotoItem*) _layout->itemAt(i)->widget();
        if (rubberBandRect.contains  (item->geometry()) ||
            rubberBandRect.intersects(item->geometry()))
            _selected << item;
    }
    updateSelection();
}

void PhotoView::updateSelection()
{
    for (int i = 0; i < _layout->count(); ++i)
    {
        PhotoItem* item = (PhotoItem*) _layout->itemAt(i)->widget();
        item->setSelected(_selected.contains(item));
    }
    emit selectionChanged(_selected);
}

PhotoItem* PhotoView::getClickedItem(const QPoint& point)
{
    for (int i = 0; i < _layout->count(); ++i)
    {
        PhotoItem* item = (PhotoItem*) _layout->itemAt(i)->widget();
        if (item->geometry().contains(point))
            return item;
    }
    return 0;
}

void PhotoView::onItemSelected(bool selected)
{
    PhotoItem* item = (PhotoItem*) sender();
    if (selected)
        _selected << item;
    else
        _selected.removeAt(_selected.indexOf(item));
}

void PhotoView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton && _rubberBand != 0)
        _rubberBand->hide();
}


////////////////////////////////////////////////////////////////////////////////////
bool PhotoItemLessTitle::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
    return  ((PhotoItem*) lhs->widget())->getPhoto()->getTitle() <
            ((PhotoItem*) rhs->widget())->getPhoto()->getTitle();
}

bool PhotoItemGreaterTitle::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
    return  ((PhotoItem*) lhs->widget())->getPhoto()->getTitle() >
            ((PhotoItem*) rhs->widget())->getPhoto()->getTitle();
}

bool PhotoItemLessTime::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
    return  ((PhotoItem*) lhs->widget())->getPhoto()->getTimeTaken() <
            ((PhotoItem*) rhs->widget())->getPhoto()->getTimeTaken();
}

bool PhotoItemGreaterTime::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const {
    return  ((PhotoItem*) lhs->widget())->getPhoto()->getTimeTaken() >
            ((PhotoItem*) rhs->widget())->getPhoto()->getTimeTaken();
}

