#include "ExifView.h"
//#include "QExifImageHeader.h"
#include "Photo.h"
#include <QDebug>

ExifView::ExifView(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
}

void ExifView::onPhotoSelected(Photo* photo)
{
    _photo = photo;
//    QExifImageHeader exif(_photo->getFilePath());
//    for (QExifImageHeader::ImageTag tag: exif.imageTags())
//        qDebug() << exif.value(tag).toString();
//    for (QExifImageHeader::ExifExtendedTag tag: exif.extendedTags())
//        qDebug() << exif.value(tag).toString();
//    for (QExifImageHeader::GpsTag tag: exif.gpsTags())
//        qDebug() << exif.value(tag).toString();
}
