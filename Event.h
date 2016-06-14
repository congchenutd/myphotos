#ifndef EVENT_H
#define EVENT_H

#include "Persistable.h"

#include <QDate>
#include <QString>

class Event: public Persistable
{
public:
    Event(int id, const QString& name, const QDate& date);

    QString getName()   const;
    QDate   getDate()   const;

    void setName(const QString& name);
    void setDate(const QDate& date);

private:
    QString _name;
    QDate   _date;
};

#endif // EVENT_H
