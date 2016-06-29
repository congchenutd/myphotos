#include "People.h"
#include "PeopleDAO.h"
#include "PhotoDAO.h"
#include "Event.h"
#include "EventDAO.h"
#include "Tag.h"
#include "Thumbnail.h"
#include "Library.h"
#include "Exif.h"
#include <QDateTime>
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QSqlError>

PhotoDAO* PhotoDAO::getInstance()
{
    static PhotoDAO instance;
    return &instance;
}

Photo* PhotoDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select Title, FilePath, Time, Info, Address from Photos where id = %1").arg(id));
    if (query.next() == 0)
        return 0;

    // load the photo itself
    Photo* photo = new Photo(id,
                             query.value(0).toString(),
                             query.value(1).toString(),
                             QDateTime::fromString(query.value(2).toString(), Qt::ISODate));
    photo->setExif(Exif(query.value(3).toString()));
    photo->setAddress(query.value(4).toString());

    // load its relationships
    Library* library = Library::getInstance();
    query.exec(tr("select People.Name \
                   from Photos, People, PhotoPeople \
                   where Photos.ID = %1 and Photos.ID = PhotoID and People.ID = PeopleID").arg(id));
    while (query.next())
    {
        QString peopleName  = query.value(0).toString();
        photo->addPeople(library->getPeople(peopleName));
    }

    query.exec(tr("select Tags.Name \
                   from Photos, Tags, PhotoTag \
                   where PhotoID = %1 and Photos.ID = PhotoID and Tags.ID = TagID").arg(id));
    while (query.next())
        photo->addTag(library->getTag(query.value(0).toString()));

    query.exec(tr("select Events.Name \
                   from Photos, Events, PhotoEvent \
                   where PhotoID = %1 and Photos.ID = PhotoID and Events.ID = EventID").arg(id));
    if (query.next())
        photo->setEvent(library->getEvent(query.value(0).toString()));

    query.exec(tr("select Thumbnails.FilePath \
                   from Photos, Thumbnails, PhotoThumbnail \
                   where PhotoID = %1 and Photos.ID = PhotoID and Thumbnails.ID = ThumbnailID").arg(id));
    if (query.next())
        photo->setThumbnail(library->getThumbnail(query.value(0).toString()));

    return photo;
}

void PhotoDAO::remove(Persistable* persistable)
{
    // remove thumbnail
    Photo* photo = static_cast<Photo*>(persistable);
    if (Thumbnail* thumbnail = photo->getThumbnail())
    {
        thumbnail->destroy();
        photo->setThumbnail(0);
    }

    // remove itself and relationships
    DAO::remove(persistable);
}

PhotoDAO::PhotoDAO(): DAO("Photos") {}

void PhotoDAO::update(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.prepare("update Photos set Title = :Title, FilePath = :FilePath, \
                   Time = :Time, Info = :Info, Address = :Address where ID = :ID");
    query.bindValue(":Title",       photo->getTitle());
    query.bindValue(":FilePath",    photo->getFilePath());
    query.bindValue(":Time",        photo->getTimeTaken().toString(Qt::ISODate));
    query.bindValue(":Info",        photo->getExif().toJson());
    query.bindValue(":Address",     photo->getAddress());
    query.bindValue(":ID",          photo->getID());
    query.exec();

    updateRelationships(photo);
}

void PhotoDAO::insert(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into Photos values (%1, \"%2\", \"%3\", \"%4\", \"%5\", \"%6\")")
               .arg(photo->getID())
               .arg(photo->getTitle())
               .arg(photo->getFilePath())
               .arg(photo->getTimeTaken().toString(Qt::ISODate))
               .arg(photo->getExif().toJson())
               .arg(photo->getAddress()));

    insertRelationships(photo);
}

void PhotoDAO::removeRelationships(Persistable* persistable)
{
    Photo* photo = static_cast<Photo*>(persistable);
    QSqlQuery query;
    query.exec(tr("delete from PhotoPeople      where PhotoID = %1").arg(photo->getID()));
    query.exec(tr("delete from PhotoTag         where PhotoID = %1").arg(photo->getID()));
    query.exec(tr("delete from PhotoEvent       where PhotoID = %1").arg(photo->getID()));
    query.exec(tr("delete from PhotoThumbnail   where PhotoID = %1").arg(photo->getID()));
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

    if (photo->getEvent() != 0)
        query.exec(tr("insert into PhotoEvent values (%1, %2)")
                   .arg(photo->getID())
                   .arg(photo->getEvent()->getID()));

    if (photo->getThumbnail() != 0)
        query.exec(tr("insert into PhotoThumbnail values (%1, %2)")
                   .arg(photo->getID())
                   .arg(photo->getThumbnail()->getID()));
}
