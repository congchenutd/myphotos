#ifndef PHOTO_H
#define PHOTO_H

#include "Persistable.h"

#include <QDateTime>
#include <QMap>
#include <QSet>
#include <QString>
#include <QStringList>

class People;
class Event;
class Tag;
class PhotoDAO;
class Thumbnail;
class PhotoInfo;

/**
 * A Photo object represents a photo
 * A Photo may have 0 or 1 event, 1 thumbnail, 0-n tags, and 0-n people.
 * NOTE: all the relationship between Photo and others are managed by Photo and PhotoDAO
 */
class Photo: public Persistable
{
public:
    Photo(int id, const QString& title, const QString& path, const QDateTime& time);

    QString                 getTitle()          const;
    QString                 getFilePath()       const;
    QDateTime               getTimeTaken()      const;
    Event*                  getEvent()          const;
    Thumbnail*              getThumbnail()      const;
    QMap<QString, Tag*>     getTags()           const;
    QMap<QString, People*>  getPeople()         const;
    QSet<QString>           getTagNames()       const;
    QSet<QString>           getPeopleNames()    const;
    PhotoInfo*              getInfo()           const;

    bool isVideo() const;
    bool exists () const;

    void setTitle       (const QString& title);
    void setFilePath    (const QString& filePath);
    void setTimeTaken   (const QDateTime& time);
    void addTag         (Tag*       tag);
    void addPeople      (People*    people);
    void setEvent       (Event*     event);
    void setThumbnail   (Thumbnail* thumbnail);
    void removeTag      (const QString& tagValue);
    void removePeople   (const QString& name);
    void setInfo        (PhotoInfo* info);

private:
    QString                 _title;
    QString                 _filePath;
    QDateTime               _time;
    QMap<QString, Tag*>     _tags;
    QMap<QString, People*>  _people;
    Event*                  _event;
    Thumbnail*              _thumbnail;
    PhotoInfo*              _info;
};

#endif // PHOTO_H
