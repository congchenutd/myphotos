#include "Event.h"
#include "EventDAO.h"
#include "PhotoDAO.h"
#include <QVariant>
#include <QSqlQuery>

EventDAO* EventDAO::getInstance()
{
    static EventDAO instance;
    return &instance;
}

Event* EventDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select Name, Date from Events where id = %1").arg(id));
    return query.next() ? new Event(id, query.value(0).toString(),
                                    QDate::fromString(query.value(1).toString(), Qt::ISODate))
                        : 0;

//    // load relationships
//    query.exec(tr("select PhotoID from PhotoEvent where EventID = %1").arg(id));
//    while (query.next())
//    {
//        int photoID = query.value(0).toInt();
//        result->addPhoto(PhotoDAO::getInstance()->load(photoID));
//    }
}

EventDAO::EventDAO(): DAO("Events") {}

void EventDAO::update(Persistable* persistable)
{
    Event* event = static_cast<Event*>(persistable);
    QSqlQuery query;
    query.exec(tr("update Events set Name = \"%1\", Date = \"%2\" where ID = %3")
                .arg(event->getName())
                .arg(event->getDate().toString("yyyy-MM-dd"))
                .arg(event->getID()));
}

void EventDAO::insert(Persistable* persistable)
{
    Event* event = static_cast<Event*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into Events values (%1, \"%2\", \"%3\")")
               .arg(event->getID())
               .arg(event->getName())
               .arg(event->getDate().toString("yyyy-MM-dd")));
}


