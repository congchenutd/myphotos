#include "LibraryDAO.h"
#include "PeopleDAO.h"
#include "EventDAO.h"
#include "PhotoDAO.h"
#include "Photo.h"
#include "Event.h"
#include "People.h"
#include "Library.h"
#include "TagDAO.h"
#include "ThumbnailDAO.h"
#include "Settings.h"

#include <QDir>
#include <QSqlQuery>
#include <QVariant>

LibraryDAO* LibraryDAO::getInstance()
{
    static LibraryDAO instance;
    return &instance;
}

void LibraryDAO::load(Library* library)
{
    QSqlQuery query;

    query.exec("select ID from People Order By ID");
    while (query.next())
        library->addPeople(PeopleDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from Tags Order By ID");
    while (query.next())
        library->addTag(TagDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from Events Order By ID");
    while (query.next())
        library->addEvent(EventDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from Thumbnails Order By ID");
    while (query.next())
        library->addThumbnail(ThumbnailDAO::getInstance()->load(query.value(0).toInt()));

    // NOTE: Load photos the last to ensure that their relationships are already loaded
    query.exec("select ID from Photos Order By ID");
    while (query.next())
        library->addPhoto(PhotoDAO::getInstance()->load(query.value(0).toInt()));
}

void LibraryDAO::save(Library* library)
{
    if (QSqlDatabase::database().transaction())
    {
        for (Tag* tag: library->getAllTags())
            tag->save();
        for (People* people: library->getAllPeople())
            people->save();
        for (Event* event: library->getAllEvents())
            event->save();
        for (Thumbnail* thumbnail: library->getAllThumbnails())
            thumbnail->save();

        // NOTE: always save photos the last, because the related objects need to be persisted first
        for (Photo* photo: library->getAllPhotos())
            photo->save();
        QSqlDatabase::database().commit();
    }
}

void LibraryDAO::clean()
{
    removeUnusedTags();
    removeUnusedEvents();
    removeUnusedPeople();
}

/**
 * Clean up unused tags
 */
void LibraryDAO::removeUnusedTags()
{
    QSqlQuery query;
    query.exec(tr("delete from Tags where ID in (select ID from Tags \
                  where ID not in (select TagID from PhotoTag))"));
}

void LibraryDAO::removeUnusedEvents()
{
    QSqlQuery query;
    query.exec(tr("delete from Events where ID in (select ID from Events \
                  where ID not in (select EventID from PhotoEvent))"));
}

void LibraryDAO::removeUnusedPeople()
{
    QSqlQuery query;
    query.exec(tr("delete from People where ID in (select ID from People \
                  where ID not in (select PeopleID from PhotoPeople))"));
}

LibraryDAO::LibraryDAO()
{
    createTables();
}

void LibraryDAO::createTables()
{
    QSqlQuery query;
    query.exec("PRAGMA foreign_keys = ON");
    query.exec("create table People ( \
                    ID int primary key, \
                    Name varchar \
                )");
    query.exec("create table Photos ( \
                    ID int primary key, \
                    Title varchar, \
                    FilePath varchar, \
                    Time date \
                )");
    query.exec("create table Events ( \
                    ID int primary key, \
                    Name varchar, \
                    Date varchar \
                )");
    query.exec("create table Tags ( \
                    ID int primary key, \
                    Name varchar \
                )");
    query.exec("create table Thumbnails ( \
                    ID int primary key, \
                    FilePath varchar \
               )");
    query.exec("create table PhotoThumbnail ( \
                    PhotoID     int references Photos    (ID) on delete cascade on update cascade, \
                    ThumbnailID int references Thumbnails(ID) on delete cascade on update cascade, \
                    primary key (PhotoID, ThumbnailID) \
                )");
    query.exec("create table PhotoPeople ( \
                    PhotoID  int references Photos(ID) on delete cascade on update cascade, \
                    PeopleID int references People(ID) on delete cascade on update cascade, \
                    primary key (PhotoID, PeopleID) \
                )");
    query.exec("create table PhotoEvent ( \
                    PhotoID int references Photos(ID) on delete cascade on update cascade, \
                    EventID int references Events(ID) on delete cascade on update cascade, \
                    primary key (PhotoID, EventID) \
                )");
    query.exec("create table PhotoTag ( \
                    PhotoID int references Photos(ID) on delete cascade on update cascade, \
                    TagID   int references Tags  (ID) on delete cascade on update cascade, \
                    primary key (PhotoID, TagID) \
                )");
}
