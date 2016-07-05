#ifndef PHOTO_H
#define PHOTO_H

#include "Persistable.h"
#include "Exif.h"
#include "Geocoder.h"

#include <QDateTime>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QGeoCoordinate>
#include <QFileInfo>

class People;
class Event;
class Tag;
class PhotoDAO;
class Thumbnail;
class Cluster;

/**
 * A Photo object represents a photo
 * A Photo may have 0 or 1 event, 1 thumbnail, 0-n tags, and 0-n people.
 * NOTE: all the relationship between Photo and others are managed by Photo and PhotoDAO
 */
class Photo: public Persistable
{
public:
    Photo(int id, const QString& title, const QString& path, const QDateTime& time);
    static Photo* fromFileInfo(const QFileInfo& fileInfo);

    QString                 getTitle()          const;
    QString                 getFilePath()       const;
    QDateTime               getTimeTaken()      const;
    Event*                  getEvent()          const;
    Thumbnail*              getThumbnail()      const;
    QMap<QString, Tag*>     getTags()           const;
    QMap<QString, People*>  getPeople()         const;
    QSet<QString>           getTagNames()       const;
    QSet<QString>           getPeopleNames()    const;
    Exif                    getExif()           const;
    QString                 getAddress()        const;
    QGeoCoordinate          getCoordinates()    const;
    Cluster*                getCluster()        const;
    bool                    isFavorite()        const;

    bool isVideo() const;
    bool exists () const;
    bool colocatedWith(const Photo* another) const;

    void setTitle       (const QString& title);
    void setFilePath    (const QString& filePath);
    void setTimeTaken   (const QDateTime& time);
    void addTag         (Tag*       tag);
    void addPeople      (People*    people);
    void setEvent       (Event*     event);
    void setThumbnail   (Thumbnail* thumbnail);
    void removeTag      (const QString& tagValue);
    void removePeople   (const QString& name);
    void setExif        (const Exif& info);
    void setAddress     (const QString& address);
    void setCluster     (Cluster* cluster);
    void setFavorite    (bool favorite);

private:
    QString                 _title;
    QString                 _filePath;
    QDateTime               _time;
    QMap<QString, Tag*>     _tags;
    QMap<QString, People*>  _people;
    Event*                  _event;
    Thumbnail*              _thumbnail;
    Exif                    _exif;
    QString                 _address;
    Cluster*                _cluster;
    bool                    _favorite;

    static const int PROXIMITY = 500;
};

#endif // PHOTO_H
