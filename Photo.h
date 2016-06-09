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

/**
 * A Photo object represents a photo
 * A Photo may have 0 or 1 event, 1 thumbnail, 0-n tags, and 0-n people.
 * NOTE: all the relationship between Photo and others are managed by Photo and PhotoDAO
 */
class Photo: public Persistable
{
public:
    Photo(int id, const QString& title, const QString& path, const QDateTime& time);

    QString         getTitle()      const;
    QString         getFilePath()   const;
    QDateTime       getTimeTaken()  const;
    QList<Tag *>    getTags()       const;
    QSet<QString>   getTagValues()  const;
    QList<People*>  getPeople()     const;
    QSet<QString>   getPeopleNames() const;
    Event*          getEvent()      const;
    Thumbnail*      getThumbnail()  const;

    void setTitle       (const QString& title);
    void setFilePath    (const QString& filePath);
    void setTimeTaken   (const QDateTime& time);
    void addTag         (Tag*       tag);
    void addPeople      (People*    people);
    void setEvent       (Event*     event);
    void setThumbnail   (Thumbnail* thumbnail);
    void removeTag      (const QString& tagValue);
    void removePeople   (const QString& name);

private:
    QString                 _title;
    QString                 _filePath;
    QDateTime               _time;
    QMap<QString, Tag*>     _tags;
    QMap<QString, People*>  _people;
    Event*                  _event;
    Thumbnail*              _thumbnail;
};

#endif // PHOTO_H
