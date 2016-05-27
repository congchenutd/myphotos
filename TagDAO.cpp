#include "TagDAO.h"
#include <QSqlQuery>

TagDAO* TagDAO::getInstance()
{
    static TagDAO instance;
    return &instance;
}

Tag* TagDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select Name from Tags where id = %1").arg(id));
    return query.next() ? new Tag(id, query.value(0).toString())
                        : 0;
}

TagDAO::TagDAO()
    : DAO("Tags")
{}

void TagDAO::update(Persistable* persistable)
{
    Tag* tag = static_cast<Tag*>(persistable);
    QSqlQuery query;
    query.exec(tr("update Tags set Name = \"%1\" where ID = %2")
               .arg(tag->getName())
               .arg(tag->getID()));
}

void TagDAO::insert(Persistable* persistable)
{
    Tag* tag = static_cast<Tag*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into Tags values(%1, \"%2\")")
               .arg(tag->getID())
               .arg(tag->getName()));
}
