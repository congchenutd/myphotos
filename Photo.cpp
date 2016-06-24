#include "Event.h"
#include "People.h"
#include "Photo.h"
#include "PhotoDAO.h"
#include "Tag.h"
#include "PhotoInfo.h"

#include <QFileInfo>
#include <QProcess>
#include <QFile>

Photo::Photo(int id, const QString& title, const QString& path, const QDateTime& time)
    : Persistable(id, PhotoDAO::getInstance()),
      _title(title),
      _filePath(path),
      _time(time),
      _event(0),
      _thumbnail(0),
      _info(new PhotoInfo)
{}

QString                 Photo::getTitle()       const { return _title;      }
QString                 Photo::getFilePath()    const { return _filePath;   }
QDateTime               Photo::getTimeTaken()   const { return _time;       }
Event*                  Photo::getEvent()       const { return _event;      }
Thumbnail*              Photo::getThumbnail()   const { return _thumbnail;  }
QMap<QString, Tag*>     Photo::getTags()        const { return _tags;   }
QMap<QString, People*>  Photo::getPeople()      const { return _people; }
QSet<QString>           Photo::getTagNames()    const { return getTags()  .keys().toSet(); }
QSet<QString>           Photo::getPeopleNames() const { return getPeople().keys().toSet(); }
PhotoInfo*              Photo::getInfo()        const { return _info;       }

bool Photo::isVideo() const
{
    QStringList videos;
    videos << "mp4" << "avi" << "mpg" << "mov";
    QString extension = QFileInfo(getFilePath()).suffix().toLower();
    return videos.contains(extension);
}

bool Photo::exists() const {
    return QFile::exists(getFilePath());
}

void Photo::setTitle(const QString& title)  {
    _title = title;
}

void Photo::setFilePath(const QString& filePath) {
    QFile::rename(_filePath, filePath);
    _filePath = filePath;
}

void Photo::setTimeTaken(const QDateTime& time)
{
    _time = time;

    // modify date and time of the physical file
    QProcess::execute("touch",
                      QStringList() << "-t" << time.toString("yyyyMMddhhmm") << getFilePath());
}

void Photo::addTag(Tag* tag) {
    _tags.insert(tag->getName(), tag);
}

void Photo::addPeople(People* people) {
    _people.insert(people->getName(), people);
}

void Photo::setEvent(Event* event) {
    _event = event;
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

void Photo::setInfo(PhotoInfo* info) {
    _info = info;
}
