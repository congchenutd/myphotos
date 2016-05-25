#include "Photo.h"

Photo::Photo()
{

}

QString Photo::getTitle() const {
    return _title;
}

QString Photo::getFileName() const {
    return _fileName;
}

QStringList Photo::getTags() const {
    return _tags;
}

QList<People*> Photo::getPeople() const {
    return _people;
}

void Photo::setTitle(const QString& title)  {
    _title = title;
}

void Photo::setFileName(const QString& fileName) {
    _fileName = fileName;
    // TODO
}

void Photo::addTag(const QString& tag) {
    if (!_tags.contains(tag))
        _tags.append(tag);
}

void Photo::addPeople(People* people) {
    if (!_people.contains(people))
        _people.append(people);
}
