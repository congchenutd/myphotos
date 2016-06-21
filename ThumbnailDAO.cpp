#include "Persistable.h"
#include "ThumbnailDAO.h"
#include "Settings.h"
#include <QFile>
#include <QSqlQuery>

ThumbnailDAO::ThumbnailDAO()
    : DAO("Thumbnails")
{}

ThumbnailDAO* ThumbnailDAO::getInstance()
{
    static ThumbnailDAO instance;
    return &instance;
}

Thumbnail* ThumbnailDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select FilePath from Thumbnails where id = %1").arg(id));
    return query.next() ? new Thumbnail(id, query.value(0).toString())
                        : 0;
}

void ThumbnailDAO::remove(Persistable* persistable)
{
    Thumbnail* thumbnail = static_cast<Thumbnail*>(persistable);
    QFile(thumbnail->getFilePath()).remove();   // delete thumbnail file
    DAO::remove(persistable);                   // delete thumbnail from db
}

void ThumbnailDAO::update(Persistable* persistable)
{
    Thumbnail* thumbnail = static_cast<Thumbnail*>(persistable);
    QSqlQuery query;
    query.exec(tr("update Thumbnails set FilePath = \"%1\" where ID = %2")
               .arg(thumbnail->getFilePath())
               .arg(thumbnail->getID()));
}

void ThumbnailDAO::insert(Persistable* persistable)
{
    Thumbnail* thumbnail = static_cast<Thumbnail*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into Thumbnails values(%1, \"%2\")")
               .arg(thumbnail->getID())
               .arg(thumbnail->getFilePath()));
}
