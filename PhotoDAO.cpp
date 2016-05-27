#include "People.h"
#include "PeopleDAO.h"
#include "PhotoDAO.h"
#include "Event.h"
#include "EventDAO.h"
#include "Tag.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>

PhotoDAO* PhotoDAO::getInstance()
{
    static PhotoDAO instance;
    return &instance;
}

Photo* PhotoDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select Title, FilePath, Time from Photos where id = %1").arg(id));
    if (query.next() == 0)
        return 0;

    return new Photo(id,
                     query.value(0).toString(),
                     query.value(1).toString(),
                     QDateTime::fromString(query.value(2).toString(), Qt::ISODate));
}

PhotoDAO::PhotoDAO(): DAO("Photos") {}

void PhotoDAO::update(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.exec(tr("update Photos set Title = \"%1\", FilePath = \"%2\", Time = \"%3\", where ID = %4")
               .arg(photo->getTitle())
               .arg(photo->getFilePath())
               .arg(photo->getTimeTaken().toString(Qt::ISODate))
               .arg(photo->getID()));

    updateRelationships(photo);
}

void PhotoDAO::insert(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into Photos values (%1, \"%2\", \"%3\", \"%4\")")
               .arg(photo->getID())
               .arg(photo->getTitle())
               .arg(photo->getFilePath())
               .arg(photo->getTimeTaken().toString(Qt::ISODate)));

    insertRelationships(photo);
}

void PhotoDAO::removeRelationships(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.exec(tr("delete from PhotoPeople where PhotoID = %1").arg(photo->getID()));
    query.exec(tr("delete from PhotoTag    where PhotoID = %1").arg(photo->getID()));
    query.exec(tr("delete from PhotoEvent  where PhotoID = %1").arg(photo->getID()));
}

void PhotoDAO::insertRelationships(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;

    for (People* people: photo->getPeople())
        query.exec(tr("insert into PhotoPeople values (%1, %2)")
                   .arg(photo->getID())
                   .arg(people->getID()));

    for (Tag* tag: photo->getTags())
        query.exec(tr("insert into PhotoTag values (%1, %2)")
                   .arg(photo->getID())
                   .arg(tag->getID()));

    query.exec(tr("insert into PhotoEvent values (%1, %2)")
                .arg(photo->getID())
                .arg(photo->getEvent()->getID()));
}
