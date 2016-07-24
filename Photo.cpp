#include "Event.h"
#include "People.h"
#include "Photo.h"
#include "PhotoDAO.h"
#include "Tag.h"
#include "Thumbnail.h"
#include "ThumbnailGenerator.h"

#include <QFileInfo>
#include <QProcess>
#include <QFile>
#include <QDir>
#include <cmath>

Photo::Photo(int id, const QString& title, const QString& path, const QDateTime& time)
    : Persistable(id, PhotoDAO::getInstance()),
      _title    (title),
      _filePath (path),
      _time     (time),
      _event    (0),
      _thumbnail(0),
      _favorite (false)
{}

Photo* Photo::fromFileInfo(const QFileInfo &fileInfo) {
    return new Photo(PhotoDAO::getInstance()->getNextID(),
                     fileInfo.baseName(),
                     fileInfo.absoluteFilePath(),
                     fileInfo.lastModified());
}

QString                 Photo::getTitle()       const { return _title;      }
QString                 Photo::getFilePath()    const { return _filePath;   }
QDateTime               Photo::getTimeTaken()   const { return _time;       }
Event*                  Photo::getEvent()       const { return _event;      }
Thumbnail*              Photo::getThumbnail()   const { return _thumbnail;  }
QMap<QString, Tag*>     Photo::getTags()        const { return _tags;   }
QMap<QString, People*>  Photo::getPeople()      const { return _people; }
QSet<QString>           Photo::getTagNames()    const { return getTags()  .keys().toSet(); }
QSet<QString>           Photo::getPeopleNames() const { return getPeople().keys().toSet(); }
Exif                    Photo::getExif()        const { return _exif;       }
QString                 Photo::getAddress()     const { return _address;    }
Cluster*                Photo::getCluster()     const { return _cluster;    }
bool                    Photo::isFavorite()     const { return _favorite;   }

QGeoCoordinate Photo::getCoordinates() const
{
    QString latitude    = getExif().getValue("GPS Latitude");
    QString longitude   = getExif().getValue("GPS Longitude");
    return QGeoCoordinate(latitude.toDouble(), longitude.toDouble());
}

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

bool Photo::colocatedWith(const Photo* another) const {
    return getCoordinates().distanceTo(another->getCoordinates()) * 100 < PROXIMITY;
}

void Photo::setTitle(const QString& title, bool emitChange)
{
    _title = title;

    // change file name to title
    QFileInfo fileInfo(getFilePath());
    QString newPath = fileInfo.absolutePath() + QDir::separator() + title + "." + fileInfo.suffix();
    setFilePath(newPath);

    if (emitChange)
        emit changed();
}

void Photo::setFilePath(const QString& filePath)
{
    if (QFile::rename(_filePath, filePath))
    {
        _filePath = filePath;

        // rename thumbnail file
        if (Thumbnail* thumbnail = getThumbnail())
        {
            QFile::remove(thumbnail->getFilePath());
            thumbnail->setFilePath(createThumbnailFile(this));
        }
    }
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

void Photo::setEvent(Event* event)
{
    _event = event;
    if (_event != 0)
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

void Photo::setExif(const Exif& info) {
    _exif = info;
}

void Photo::setAddress(const QString &address) {
    _address = address;
}

void Photo::setCluster(Cluster* cluster) {
    _cluster = cluster;
}

void Photo::setFavorite(bool favorite) {
    _favorite = favorite;
}

void Photo::renameBasedOnEvent(int idx, int eventSize)
{
    QFileInfo fileInfo(getFilePath());
    QString date = fileInfo.lastModified().toString("yyyy-MM-dd");
    QString index = QString("%1").arg(QString::number(idx), log10(eventSize) + 1, '0');  // pad with 0
    QString title = date + "-" + getEvent()->getName() + "-" + index;
    setTitle(title);
}
