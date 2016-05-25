#ifndef PHOTO_H
#define PHOTO_H

#include <QString>
#include <QStringList>

class People;
class Event;

class Photo
{
public:
    Photo();

    QString         getTitle()      const;
    QString         getFileName()   const;
    QStringList     getTags()       const;
    QList<People*>  getPeople()     const;
    Event*          getEvent()      const;

    void setTitle   (const QString& title);
    void setFileName(const QString& fileName);
    void addTag     (const QString& tag);
    void addPeople  (People* people);
    void setEvent   (Event* event);

private:
    QString         _title;
    QString         _fileName;
    QStringList     _tags;
    QList<People*>  _people;
    Event*          _event;
};

#endif // PHOTO_H
