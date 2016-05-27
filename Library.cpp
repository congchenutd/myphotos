#include "Event.h"
#include "Library.h"
#include "People.h"
#include "Photo.h"
#include "Settings.h"
#include "LibraryDAO.h"
#include "Tag.h"

#include <QSqlQuery>
#include <QDir>
#include <QDebug>

Library* Library::getInstance()
{
    static Library instance;
    return &instance;
}

QStringList Library::getMonitoredFolders() const {
    return Settings::getInstance()->getMonitoredFolders();
}

void Library::setMonitoredFolders(const QStringList& list) {
    Settings::getInstance()->setMonitoredFolders(list);
}

void Library::scan()
{
    QStringList folders = Settings::getInstance()->getMonitoredFolders();
    for (const QString& folder: folders)
    {
        QDir dir(folder);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setSorting(QDir::Size | QDir::Reversed);
        dir.setNameFilters(Settings::getInstance()->getMonitoredFileTypes().split(";"));
        for (const QFileInfo& info: dir.entryInfoList())
        {
            qDebug() << info.filePath();
        }
    }
}

void Library::save() {
    _dao->save(this);
}

QMap<int, People*> Library::getAllPeople() const {
    return _people;
}

QMap<int, Tag*> Library::getAllTags() const {
    return _tags;
}

QMap<int, Event*> Library::getAllEvents() const {
    return _events;
}

QMap<int, Photo*> Library::getAllPhotos() const {
    return _photos;
}

People* Library::getPeople(int id) {
    return _people.find(id).value();
}

Tag* Library::getTag(int id) {
    return _tags.find(id).value();
}

Event* Library::getEvent(int id) {
    return _events.find(id).value();
}

Photo* Library::getPhoto(int id) {
    return _photos.find(id).value();
}

void Library::addPhoto(Photo* photo) {
    if (photo != 0)
        _photos.insert(photo->getID(), photo);
}

void Library::addEvent(Event* event) {
    if (event != 0)
        _events.insert(event->getID(), event);
}

Library::Library()
{
    _dao = LibraryDAO::getInstance();
    _dao->load(this);
}

void Library::addPeople(People* people) {
    if (people != 0)
        _people.insert(people->getID(), people);
}

void Library::addTag(Tag* tag) {
    if (tag != 0)
        _tags.insert(tag->getID(), tag);
}
