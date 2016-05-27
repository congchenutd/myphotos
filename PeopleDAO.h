#ifndef PEOPLEDAO_H
#define PEOPLEDAO_H

#include "People.h"
#include "DAO.h"

class PeopleDAO: public DAO
{
public:
    static PeopleDAO* getInstance();

    People* load(int id) const;

private:
    PeopleDAO();
    void update(Persistable* persistable);
    void insert(Persistable* persistable);
};

#endif // PEOPLEDAO_H
