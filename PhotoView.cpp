#include "PhotoView.h"
#include "FlowLayout.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoItem.h"
#include <QFileSystemModel>
#include <QLabel>
#include <algorithm>

PhotoView::PhotoView(QWidget *parent) :
    QWidget(parent)
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

//    PhotoItemComparator* comparator = getComparator(byWhat, ascending);
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
//    std::sort(items.begin(), items.end(), *comparator);

    for (PhotoItem* item: items)
        _layout->addWidget(item);
    _layout->update();
}

void PhotoView::addPhoto(Photo* photo)
{
    PhotoItem* photoItem = new PhotoItem(photo);
    connect(photoItem, SIGNAL(itemSelected(Photo*)), this, SIGNAL(photoSelected(Photo*)));
    _layout->addWidget(photoItem);
}

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

PhotoItemComparator* PhotoView::getComparator(const QString& byWhat, bool ascending)
{
    if (byWhat == "Title") {
        if (ascending)  return new PhotoItemLessTitle();
        else            return new PhotoItemGreaterTitle();
    }
    if (byWhat == "Time") {
        if (ascending)  return new PhotoItemLessTime();
        else            return new PhotoItemGreaterTime();
    }
    return 0;
}
