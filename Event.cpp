#include "Event.h"
#include "EventDAO.h"

Event::Event(int id, const QString& name, const QDate& date)
    : Persistable(id, EventDAO::getInstance()), _name(name), _date(date)
{}

QString Event::getName() const {
    return _name;
}

QDate Event::getDate() const {
    return _date;
}

QList<Photo*> Event::getPhotos() const {
    return _photos;
}

void Event::setName(const QString& name) {
    _name = name;
}

void Event::setDate(const QDate& date) {
    _date = date;
}

void Event::addPhoto(Photo* photo) {
    if (!_photos.contains(photo))
        _photos << photo;
}
