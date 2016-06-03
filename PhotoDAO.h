#ifndef PHOTODAO_H
#define PHOTODAO_H

#include "DAO.h"
#include "Photo.h"

class Photo;

class PhotoDAO : public DAO
{
public:
    static PhotoDAO* getInstance();

    Photo* load(int id) const;
    void remove(Persistable* persistable);

private:
    PhotoDAO();
    void update(Persistable* persistable);
    void insert(Persistable* persistable);
    void removeRelationships(Persistable* persistable);
    void insertRelationships(Persistable* persistable);
};

#endif // PHOTODAO_H
