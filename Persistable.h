#ifndef PERSISTABLE_H
#define PERSISTABLE_H

class DAO;

class Persistable
{
public:
    Persistable(int id, DAO* dao);

    int getID() const;
    static int getNextID();

    virtual ~Persistable() {}
    virtual void destroy();
    virtual void save();

private:
    int     _id;
    DAO*    _dao;

    static int _maxID;
};

#endif // PERSISTABLE_H
