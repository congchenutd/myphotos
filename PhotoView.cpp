#include "PhotoView.h"
#include "FlowLayout.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include <QFileSystemModel>
#include <QLabel>

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
//    QLayoutItem* child;
//    while ((child = _layout->takeAt(0)) != 0) {
//        delete child;
//    }

    for (Photo* photo: _library->getAllPhotos().values())
        addPhoto(photo);
}

void PhotoView::addPhoto(Photo* photo)
{
    QPixmap pixmap(photo->getThumbnail()->getFilePath());
    QLabel* label = new QLabel();
    label->setPixmap(pixmap);
    _layout->addWidget(label);
}
