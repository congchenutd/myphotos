#include "Event.h"
#include "EventDAO.h"
#include "Photo.h"
#include "Comparator.h"

#include <cmath>

Event::Event(int id, const QString& name, const QDate& date)
    : Persistable(id, EventDAO::getInstance()), _name(name), _date(date)
{}

QString Event::getName() const {
    return _name;
}

QDate Event::getDate() const {
    return _date;
}

void Event::setName(const QString& name) {
    _name = name;
}

void Event::setDate(const QDate& date) {
    _date = date;
}

QList<Photo*> Event::getAllPhotos() const {
    return _photos;
}

void Event::addPhoto(Photo* photo)
{
    if (photo != 0 && !_photos.contains(photo))
    {
        _photos << photo;
        qSort(_photos.begin(), _photos.end(), ComparePhotoByTime());
        renamePhotos();
    }
}

int Event::photoCount() const {
    return _photos.count();
}

void Event::renamePhotos()
{
    int paddingLength = static_cast<int>(log10(photoCount())) + 1;
    for (int i = 0; i < photoCount(); ++i)
    {
        Photo* photo = _photos.at(i);
        QFileInfo fileInfo(photo->getFilePath());
        QString date = fileInfo.lastModified().toString("yyyy-MM-dd");
        QString index = QString("%1").arg(QString::number(i), paddingLength, '0');  // pad with 0
        QString title = date + "-" + getName();
        if (photoCount() > 1)
            title += "-" + index;
        photo->setTitle(title);
    }
}
