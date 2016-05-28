#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMap>
#include <QObject>

class Photo;
class Event;
class People;
class Tag;
class LibraryDAO;
class Thumbnail;

class Library: public QObject
{
    Q_OBJECT

public:
    static Library* getInstance();

    QStringList getMonitoredFolders() const;
    void setMonitoredFolders(const QStringList &list);

    void scan();
    void save();

    QMap<QString, Photo*>       getAllPhotos()      const;
    QMap<QString, People*>      getAllPeople()      const;
    QMap<QString, Tag*>         getAllTags  ()      const;
    QMap<QString, Event*>       getAllEvents()      const;
    QMap<QString, Thumbnail*>   getAllThumbnails()  const;

    Photo*      getPhoto    (const QString& filePath);
    People*     getPeople   (const QString& name);
    Tag*        getTag      (const QString& name);
    Event*      getEvent    (const QString& name);
    Thumbnail*  getThumbnail(const QString& filePath);

    void addPhoto       (Photo*     photo);
    void addPeople      (People*    people);
    void addTag         (Tag*       tag);
    void addEvent       (Event*     event);
    void addThumbnail   (Thumbnail* thumbnail);

private:
    Library();
    ~Library() {}

signals:
    void photoAdded(Photo*);

private:
    LibraryDAO*                 _dao;
    QMap<QString, Photo*>       _photos;
    QMap<QString, People*>      _people;
    QMap<QString, Tag*>         _tags;
    QMap<QString, Event*>       _events;
    QMap<QString, Thumbnail*>   _thumbnails;
};

#endif // LIBRARY_H
