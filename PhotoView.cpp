#include "PhotoView.h"
#include "FlowLayout.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoItem.h"
#include <QFileSystemModel>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <algorithm>
#include <QAction>

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
    _sortBy = byWhat;
    _ascending = ascending;
    QList<PhotoItem*> items;
    while (!_layout->isEmpty())
        items << (PhotoItem*) _layout->takeAt(0)->widget();

    if (byWhat == "Title") {
        if (ascending)  std::sort(items.begin(), items.end(), PhotoItemLessTitle());
        else            std::sort(items.begin(), items.end(), PhotoItemGreaterTitle());
    }
    if (byWhat == "Time") {
        if (ascending)  std::sort(items.begin(), items.end(), PhotoItemLessTime());
        else            std::sort(items.begin(), items.end(), PhotoItemGreaterTime());
    }

    for (PhotoItem* item: items)
        _layout->addWidget(item);
    _layout->update();
}

QList<PhotoItem*> PhotoView::getSelectedItems() const {
    return _selected;
}

void PhotoView::addPhoto(Photo* photo)
{
    PhotoItem* photoItem = new PhotoItem(photo);
    connect(photoItem, SIGNAL(itemSelected(Photo*)), this, SIGNAL(photoSelected(Photo*)));
    connect(photoItem, SIGNAL(titleEdited(QString)), this, SLOT(sort()));
    _layout->addWidget(photoItem);
}

void PhotoView::sort() {
    sort(_sortBy, _ascending);
}

void PhotoView::mousePressEvent(QMouseEvent* event)
{
    _selectionStart = event->pos();
    if (!_rubberBand)
        _rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    _rubberBand->setGeometry(QRect(_selectionStart, QSize()));
    _rubberBand->show();

    if (event->button() == Qt::LeftButton)
        selectItems();
}

void PhotoView::mouseMoveEvent(QMouseEvent* event)
{
    _rubberBand->setGeometry(QRect(_selectionStart, event->pos()).normalized());
    selectItems();
}

void PhotoView::selectItems()
{
    QRect rubberBandRect = _rubberBand->geometry();
    _selected.clear();
    int count = _layout->count();
    for (int i = 0; i < count; ++i)
    {
        PhotoItem* item = (PhotoItem*) _layout->itemAt(i)->widget();
        bool select = item->geometry().contains(_selectionStart) ||
                      rubberBandRect.contains(item->geometry()) ||
                      rubberBandRect.intersects(item->geometry());
        item->setSelected(select);
        if (select)
            _selected << item;
    }
    emit selectionChanged(_selected);
}

void PhotoView::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() == Qt::LeftButton)
        _rubberBand->hide();
}


////////////////////////////////////////////////////////////////////////////////////
bool PhotoItemLessTitle::operator() (PhotoItem* lhs, PhotoItem* rhs) const {
    return lhs->getPhoto()->getTitle() < rhs->getPhoto()->getTitle();
}

bool PhotoItemGreaterTitle::operator() (PhotoItem* lhs, PhotoItem* rhs) const {
    return lhs->getPhoto()->getTitle() > rhs->getPhoto()->getTitle();
}

bool PhotoItemLessTime::operator() (PhotoItem* lhs, PhotoItem* rhs) const {
    return lhs->getPhoto()->getTimeTaken() < rhs->getPhoto()->getTimeTaken();
}

bool PhotoItemGreaterTime::operator() (PhotoItem* lhs, PhotoItem* rhs) const {
    return lhs->getPhoto()->getTimeTaken() > rhs->getPhoto()->getTimeTaken();
}
