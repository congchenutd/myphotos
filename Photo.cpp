#include "Event.h"
#include "People.h"
#include "Photo.h"
#include "PhotoDAO.h"
#include "Tag.h"

Photo::Photo(int id, const QString& title, const QString& path, const QDateTime& time)
    : Persistable(id, PhotoDAO::getInstance()),
      _title(title),
      _filePath(path),
      _time(time),
      _event(0),
      _thumbnail(0)
{}

QString         Photo::getTitle()       const { return _title;      }
QString         Photo::getFilePath()    const { return _filePath;   }
QDateTime       Photo::getTimeTaken()   const { return _time;       }
QList<Tag*>     Photo::getTags()        const { return _tags.values();      }
QList<People*>  Photo::getPeople()      const { return _people.values();    }
Event*          Photo::getEvent()       const { return _event;      }
Thumbnail*      Photo::getThumbnail()   const { return _thumbnail;  }

QSet<QString> Photo::getTagValues() const
{
    QSet<QString> result;
    foreach (Tag* tag, _tags)
        result.insert(tag->getName());
    return result;
}

QSet<QString> Photo::getPeopleNames() const
{
    QSet<QString> result;
    foreach (People* people, _people)
        result.insert(people->getName());
    return result;
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
    _tags.insert(tag->getName(), tag);
}

void Photo::addPeople(People* people) {
    _people.insert(people->getName(), people);
}

void Photo::setEvent(Event* event)
{
    _event = event;
    _event->addPhoto(this);
}

void Photo::setThumbnail(Thumbnail* thumbnail) {
    _thumbnail = thumbnail;
}

void Photo::removeTag(const QString& tagValue) {
    _tags.remove(tagValue);
}

void Photo::removePeople(const QString& name) {
    _people.remove(name);
}
