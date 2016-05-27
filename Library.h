#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMap>

class Photo;
class Event;
class People;
class Tag;
class LibraryDAO;

class Library
{
public:
    static Library* getInstance();

    QStringList getMonitoredFolders() const;
    void setMonitoredFolders(const QStringList &list);

    void scan();
    void save();

    QMap<int, Photo*>   getAllPhotos()  const;
    QMap<int, People*>  getAllPeople()  const;
    QMap<int, Tag*>     getAllTags  ()  const;
    QMap<int, Event*>   getAllEvents()  const;

    Photo*  getPhoto    (int id);
    People* getPeople   (int id);
    Tag*    getTag      (int id);
    Event*  getEvent    (int id);

    void addPhoto   (Photo*     photo);
    void addPeople  (People*    people);
    void addTag     (Tag*       tag);
    void addEvent   (Event*     event);

private:
    Library();
    ~Library() {}

private:
    LibraryDAO*         _dao;
    QMap<int, Photo*>   _photos;
    QMap<int, People*>  _people;
    QMap<int, Tag*>     _tags;
    QMap<int, Event*>   _events;
};

#endif // LIBRARY_H
