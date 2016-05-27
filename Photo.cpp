#include "Event.h"
#include "Photo.h"
#include "PhotoDAO.h"
#include "Tag.h"

Photo::Photo(int id, const QString& title, const QString& path, const QDateTime& time)
    : Persistable(id, PhotoDAO::getInstance()), _title(title), _filePath(path), _time(time)
{}

QString Photo::getTitle() const {
    return _title;
}

QString Photo::getFilePath() const {
    return _filePath;
}

QDateTime Photo::getTimeTaken() const {
    return _time;
}

QList<Tag*> Photo::getTags() const {
    return _tags;
}

QList<People*> Photo::getPeople() const {
    return _people;
}

Event* Photo::getEvent() const {
    return _event;
}

void Photo::setTitle(const QString& title)  {
    _title = title;
}

void Photo::setFilePath(const QString& filePath) {
    _filePath = filePath;
    // TODO
}

void Photo::setTimeTaken(const QDateTime& time) {
    _time = time;
}

void Photo::addTag(Tag* tag) {
    if (!_tags.contains(tag))
        _tags.append(tag);
}

void Photo::addPeople(People* people) {
    if (!_people.contains(people))
        _people.append(people);
}

void Photo::setEvent(Event* event)
{
    _event = event;
    _event->addPhoto(this);
}
