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

QMap<QString, People*>      Library::getAllPeople()     const { return _people;     }
QMap<QString, Tag*>         Library::getAllTags()       const { return _tags;       }
QMap<QString, Event*>       Library::getAllEvents()     const { return _events;     }
QMap<QString, Thumbnail *>  Library::getAllThumbnails() const { return _thumbnails; }
QMap<QString, Photo*>       Library::getAllPhotos()     const { return _photos;     }

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

int Library::getPhotoCount() const {
    return _photos.count() - getVideoCount();
}

int Library::getVideoCount() const
{
    int result = 0;
    foreach (Photo* photo, _photos)
        if (photo->isVideo())
            result ++;
    return result;
}

void Library::addPhoto(Photo* photo) {
    if (photo != 0)
        _photos.insert(photo->getFilePath(), photo);
}

void Library::addEvent(Event* event) {
    if (event != 0)
        _events.insert(event->getName(), event);
}

void Library::addThumbnail(Thumbnail* thumbnail) {
    if (thumbnail != 0)
        _thumbnails.insert(thumbnail->getFilePath(), thumbnail);
}

void Library::addPeople(People* people) {
    if (people != 0)
        _people.insert(people->getName(), people);
}

void Library::addTag(Tag* tag) {
    if (tag != 0)
        _tags.insert(tag->getName(), tag);
}

/**
 * Remove a photo object from the library
 * WARNING: the photo and its thumbnail are destroyed by this method, DO NOT use them afterwards
 */
void Library::removePhoto(Photo* photo)
{
    _photos.remove(photo->getFilePath());
    if (Thumbnail* thumbnail = photo->getThumbnail())
    {
        _thumbnails.remove(thumbnail->getFilePath());
        thumbnail->destroy();
    }
    photo->destroy();
}

QList<Photo*> Library::filterPhotosByTags(const QSet<QString>& tags, bool AND)
{
    if (tags.isEmpty())
        return getAllPhotos().values();

    QList<Photo*> result;
    foreach (Photo* photo, _photos)
    {
        if (AND) {
            if (photo->getTagNames().contains(tags))    // contains all the tags
                result << photo;
        }
        else {
            if (photo->getTagNames().intersects(tags))  // contains at least one of the tags
                result << photo;
        }
    }
    return result;
}

QList<Photo *> Library::filterPhotosByPeople(const QSet<QString>& people, bool AND)
{
    if (people.isEmpty())
        return getAllPhotos().values();

    QList<Photo*> result;
    foreach (Photo* photo, _photos)
    {
        if (AND) {
            if (photo->getPeopleNames().contains(people))   // contains all the people
                result << photo;
        }
        else {
            if (photo->getPeopleNames().intersects(people)) // contains at least one of the people
                result << photo;
        }
    }
    return result;
}

QList<Photo *> Library::filterPhotosByEvent(const QString& eventName)
{
    if (eventName.isEmpty())
        return getAllPhotos().values();

    QList<Photo*> result;
    foreach (Photo* photo, _photos)
        if (Event* event = photo->getEvent())
            if(event->getName() == eventName)
                result << photo;
    return result;
}
