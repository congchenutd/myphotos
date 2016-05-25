#ifndef LIBRARY_H
#define LIBRARY_H

#include <QMap>

class Photo;
class Event;
class People;

class Library
{
public:
    static Library* getInstance();

    QString         getPath()       const;
    QList<Event*>   getEvents()     const;
    QList<Photo*>   getAllPhotos()  const;

    void setPath(const QString& path);
    void addPhoto(Photo* photo);
    void addEvent(Event* event);
    void addPeople(People* people);

private:
    Library();
    void createTables();

private:
    QString _path;

    static Library* _instance;
};

#endif // LIBRARY_H
