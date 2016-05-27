#include "DAO.h"
#include "Persistable.h"

void DAO::save(Persistable* persistable)
{
    if (exists(persistable))
        update(persistable);
    else
        insert(persistable);
}

bool DAO::exists(Persistable* persistable) const
{
    QSqlQuery query;
    query.exec(tr("select * from %1 where ID = %2")
               .arg(_tableName)
               .arg(persistable->getID()));
    return query.next();
}

void DAO::remove(Persistable* persistable)
{
    QSqlQuery query;
    query.exec(tr("delete from %1 where ID = %2")
               .arg(_tableName)
               .arg(persistable->getID()));

    removeRelationships(persistable);
}

QString DAO::getTableName() const {
    return _tableName;
}

void DAO::updateRelationships(Persistable* persistable)
{
    removeRelationships(persistable);
    insertRelationships(persistable);
}

int DAO::getNextID() const
{
    QSqlQuery query;
    query.exec(tr("select max(ID) from %1").arg(_tableName));
    return query.next() ? query.value(0).toInt() : 0;
}
