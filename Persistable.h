#ifndef PERSISTABLE_H
#define PERSISTABLE_H

class DAO;

/**
 * Base class for all the model classes that need to be persisted into database
 */
class Persistable
{
public:
    Persistable(int id, DAO* dao);
    virtual ~Persistable() {}

    int getID() const;

    // Destroy this object from the database
    // WARNING: stop using this object after destroy()
    virtual void destroy();

    // Write the object into database
    virtual void save();

private:
    int     _id;
    DAO*    _dao;
};

#endif // PERSISTABLE_H
