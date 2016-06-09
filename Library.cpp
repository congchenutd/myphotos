#include "Event.h"
#include "Library.h"
#include "People.h"
#include "Photo.h"
#include "Settings.h"
#include "LibraryDAO.h"
#include "Tag.h"
#include "PhotoDAO.h"
#include "Thumbnail.h"

#include <QSqlQuery>
#include <QDir>
#include <QDebug>
#include <QApplication>

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

Library::Library() {
    _dao = LibraryDAO::getInstance();
}

void Library::load() {
    _dao->load(this);
}

void Library::save() {
    _dao->save(this);
}

void Library::clean() {
    save();
    _dao->clean();
}

QMap<QString, People*> Library::getAllPeople() const {
    return _people;
}

QMap<QString, Tag*> Library::getAllTags() const {
    return _tags;
}

QMap<QString, Event*> Library::getAllEvents() const {
    return _events;
}

QMap<QString, Thumbnail *> Library::getAllThumbnails() const {
    return _thumbnails;
}

QMap<QString, Photo*> Library::getAllPhotos() const {
    return _photos;
}

People* Library::getPeople(const QString& name) {
    return _people.contains(name) ? _people[name] : 0;
}

Tag* Library::getTag(const QString& name) {
    return _tags.contains(name) ? _tags[name] : 0;
}

Event* Library::getEvent(const QString& name) {
    return _events.contains(name) ? _events[name] : 0;
}

Thumbnail* Library::getThumbnail(const QString& filePath) {
    return _thumbnails.contains(filePath) ? _thumbnails[filePath] : 0;
}

Photo* Library::getPhoto(const QString& filePath) {
    return _photos.contains(filePath) ? _photos[filePath] : 0;
}

void Library::addPhoto(Photo* photo) {
    if (photo != 0) {
        _photos.insert(photo->getFilePath(), photo);
    }
}

void Library::addEvent(Event* event) {
    if (event != 0)
        _events.insert(event->getName(), event);
}

void Library::addThumbnail(Thumbnail* thumbnail) {
    if (thumbnail != 0)
        _thumbnails.insert(thumbnail->getFilePath(), thumbnail);
}

void Library::removePhoto(Photo* photo)
{
    _photos.remove(photo->getFilePath());
    photo->destroy();
}

QList<Photo*> Library::filterPhotosByTags(const QSet<QString>& tags, bool AND)
{
    QList<Photo*> result;
    foreach (Photo* photo, _photos)
    {
        if (AND) {
            if (photo->getTagValues().contains(tags))
                result << photo;
        }
        else {
            if (photo->getTagValues().intersects(tags))
                result << photo;
        }
    }
    return result;
}

void Library::addPeople(People* people) {
    if (people != 0)
        _people.insert(people->getName(), people);
}

void Library::addTag(Tag* tag) {
    if (tag != 0)
        _tags.insert(tag->getName(), tag);
}
