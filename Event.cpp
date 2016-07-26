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
