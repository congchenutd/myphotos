#ifndef TAGDAO_H
#define TAGDAO_H

#include "DAO.h"
#include "Tag.h"

class Tag;

class TagDAO : public DAO
{
public:
    static TagDAO* getInstance();

    Tag* load(int id) const;

private:
    TagDAO();
    void update(Persistable* persistable);
    void insert(Persistable* persistable);
};

#endif // TAGDAO_H
