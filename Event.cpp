#include "Event.h"

Event::Event(const QString& name, const QDate& date)
    : _name(name), _date(date)
{

}

QString Event::getName() const {
    return _name;
}

QDate Event::getDate() const {
    return _date;
}

QList<Photo*> Event::getPhotos() const {
    return _photos;
}
