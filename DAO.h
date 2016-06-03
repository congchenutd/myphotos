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

    QString getTableName() const;
    int getNextID() const;

    virtual Persistable* load(int id) const = 0;
    virtual void remove(Persistable* persistable);

protected:
    virtual void update(Persistable* persistable) = 0;
    virtual void insert(Persistable* persistable) = 0;
    virtual void updateRelationships(Persistable* persistable);
    virtual void removeRelationships(Persistable* ) { }
    virtual void insertRelationships(Persistable* ) { }


private:
    QString _tableName;
};

#endif // DAO_H
