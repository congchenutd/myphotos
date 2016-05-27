#ifndef EVENT_H
#define EVENT_H

#include "Persistable.h"

#include <QDate>
#include <QString>

class Photo;

class Event: public Persistable
{
public:
    Event(int id, const QString& name, const QDate& date);

    QString         getName()   const;
    QDate           getDate()   const;
    QList<Photo*>   getPhotos() const;

    void setName(const QString& name);
    void setDate(const QDate& date);
    void addPhoto(Photo* photo);

private:
    QString         _name;
    QDate           _date;
    QList<Photo*>   _photos;
};

#endif // EVENT_H
