#include "PeopleDAO.h"

#include <QSqlQuery>
#include <QVariant>

PeopleDAO::PeopleDAO(): DAO("People") {}

PeopleDAO* PeopleDAO::getInstance()
{
    static PeopleDAO instance;
    return &instance;
}

People* PeopleDAO::load(int id) const
{
    QSqlQuery query;
    query.exec(tr("select Name from People where id = %1").arg(id));
    return query.next() ? new People(id, query.value(0).toString())
                        : 0;
}

void PeopleDAO::update(Persistable* persistable)
{
    People* people = static_cast<People*>(persistable);
    QSqlQuery query;
    query.exec(tr("update People set Name = \"%1\" where ID = %2")
               .arg(people->getName())
               .arg(people->getID()));
}

void PeopleDAO::insert(Persistable* persistable)
{
    People* people = static_cast<People*>(persistable);
    QSqlQuery query;
    query.exec(tr("insert into People values(%1, \"%2\")")
               .arg(people->getID())
               .arg(people->getName()));
}
