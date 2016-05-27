#include "LibraryDAO.h"
#include "PeopleDAO.h"
#include "EventDAO.h"
#include "PhotoDAO.h"
#include "Photo.h"
#include "Event.h"
#include "People.h"
#include "Library.h"
#include "TagDAO.h"

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
    query.exec("select ID from Photos Order By Time");
    while (query.next())
        library->addPhoto(PhotoDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from People Order By Name");
    while (query.next())
        library->addPeople(PeopleDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from Tags Order By Name");
    while (query.next())
        library->addTag(TagDAO::getInstance()->load(query.value(0).toInt()));

    query.exec("select ID from Events Order By Name");
    while (query.next())
        library->addEvent(EventDAO::getInstance()->load(query.value(0).toInt()));


    query.exec(tr("select PhotoID, PeopleID from PhotoPeople"));
    while (query.next())
    {
        int photoID  = query.value(0).toInt();
        int peopleID = query.value(1).toInt();
        library->getPhoto(photoID)->addPeople(library->getPeople(peopleID));
    }

    query.exec(tr("select PhotoID, TagID from PhotoTag"));
    while (query.next())
    {
        int photoID = query.value(0).toInt();
        int tagID   = query.value(1).toInt();
        library->getPhoto(photoID)->addTag(library->getTag(tagID));
    }

    query.exec(tr("select PhotoID, EventID from PhotoEvent"));
    while (query.next())
    {
        int photoID = query.value(0).toInt();
        int eventID = query.value(1).toInt();
        library->getPhoto(photoID)->setEvent(library->getEvent(eventID));
    }
}

void LibraryDAO::save(Library* library)
{
    for (Tag* tag: library->getAllTags())
        tag->save();
    for (People* people: library->getAllPeople())
        people->save();
    for (Event* event: library->getAllEvents())
        event->save();
    for (Photo* photo: library->getAllPhotos())
        photo->save();
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
