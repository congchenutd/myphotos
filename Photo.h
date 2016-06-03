#ifndef PHOTO_H
#define PHOTO_H

#include "Persistable.h"

#include <QDateTime>
#include <QSet>
#include <QString>
#include <QStringList>

class People;
class Event;
class Tag;
class PhotoDAO;
class Thumbnail;

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

private:
    QString         _title;
    QString         _filePath;
    QDateTime       _time;
    QList<Tag*>     _tags;
    QList<People*>  _people;
    Event*          _event;
    Thumbnail*      _thumbnail;
};

#endif // PHOTO_H
