#ifndef LIBRARY_H
#define LIBRARY_H

#include <QFileInfo>
#include <QMap>
#include <QObject>

class Photo;
class Event;
class People;
class Tag;
class LibraryDAO;
class Thumbnail;

/**
 * A Library of all other objects
 */
class Library: public QObject
{
    Q_OBJECT

public:
    static Library* getInstance();

    void load();
    void save();
    void clean();

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

    int getPhotoCount() const;
    int getVideoCount() const;

    void addPhoto       (Photo*     photo);
    void addPeople      (People*    people);
    void addTag         (Tag*       tag);
    void addEvent       (Event*     event);
    void addThumbnail   (Thumbnail* thumbnail);

    void removePhoto(Photo* photo);

    QList<Photo*> filterPhotosByTags    (const QSet<QString>& tags,     bool AND);
    QList<Photo*> filterPhotosByPeople  (const QSet<QString>& people,   bool AND);
    QList<Photo*> filterPhotosByEvent   (const QString& eventName);
    QList<Photo*> filterPhotosByTitle   (const QString& title);

private:
    Library();
    ~Library() {}

private:
    LibraryDAO*                 _dao;
    QMap<QString, Photo*>       _photos;
    QMap<QString, People*>      _people;
    QMap<QString, Tag*>         _tags;
    QMap<QString, Event*>       _events;
    QMap<QString, Thumbnail*>   _thumbnails;
};

#endif // LIBRARY_H
