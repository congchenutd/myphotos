#ifndef EVENT_H
#define EVENT_H

#include <QDate>
#include <QString>

class Photo;

class Event
{
public:
    Event(const QString& name, const QDate& date);

    QString         getName()   const;
    QDate           getDate()   const;
    QList<Photo*>   getPhotos() const;

private:
    QString         _name;
    QDate           _date;
    QList<Photo*>   _photos;
};

#endif // EVENT_H
