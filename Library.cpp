#include "Event.h"
#include "Library.h"
#include "Photo.h"

#include <QSqlQuery>

Library::Library()
{
    createTables();
}

void Library::createTables()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("create table People ( \
                   ID int primary key, \
                   Name varchar \
               )");
    query.exec("create table Photos ( \
                   ID int primary key, \
                   Name varchar \
               )");
}

Library* Library::getInstance()
{
    if (_instance == 0)
        _instance = new Library;
    return _instance;
}

QList<Event*> Library::getEvents() const {
//    return _events.values();
}

QList<Photo*> Library::getAllPhotos() const
{
    QList<Photo*> result;
//    foreach(Event* event, _events)
//        result.append(event->getPhotos());
    return result;
}

void Library::setPath(const QString& path) {
    _path = path;
}

void Library::addPhoto(Photo* photo)
{

}

void Library::addEvent(Event* event) {
}

void Library::addPeople(People *people)
{

}

Library* Library::_instance = 0;
