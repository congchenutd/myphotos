#ifndef DAO_H
#define DAO_H

#include <QObject>
#include <QSqlQuery>
#include <QVariant>

class Persistable;

class DAO: public QObject
{
public:
    DAO(const QString& tableName): _tableName(tableName) {}

    void save(Persistable* persistable);
    bool exists(Persistable* persistable) const;
    void remove(Persistable* persistable);

    QString getTableName() const;

    virtual Persistable* load(int id) const = 0;

protected:
    virtual void update(Persistable* persistable) = 0;
    virtual void insert(Persistable* persistable) = 0;
    virtual void updateRelationships(Persistable* persistable);
    virtual void removeRelationships(Persistable* ) { }
    virtual void insertRelationships(Persistable* ) { }

    int getNextID() const;

private:
    QString _tableName;
};

#endif // DAO_H
