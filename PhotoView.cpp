#include "PhotoView.h"
#include "FlowLayout.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoItem.h"
#include "MainWindow.h"
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
    load();
}

void PhotoView::load()
{
    for (Photo* photo: _library->getAllPhotos().values())
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
    delete item;
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
    _selectionStart = event->pos();
    if (!_rubberBand)
        _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    _rubberBand->setGeometry(QRect(_selectionStart, QSize()));
    _rubberBand->show();

    if (event->button() == Qt::LeftButton && event->modifiers() != Qt::ShiftModifier)
        _selected.clear();
    if (PhotoItem* item = clickedItem(_selectionStart))
    {
        if (event->button() == Qt::LeftButton && _selected.contains(item))
            _selected.removeAt(_selected.indexOf(item));
        else
            _selected << item;
    }
    updateSelection();

    if (event->button() == Qt::RightButton)
    {
        if (clickedItem(_selectionStart) == 0)
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
            menu.addAction(mainWindow->getTagsAction());
        }
        menu.exec(event->globalPos());
    }
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

PhotoItem* PhotoView::clickedItem(const QPoint& point)
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

