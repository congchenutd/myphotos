#ifndef EVENTDAO_H
#define EVENTDAO_H

#include "DAO.h"
#include "Event.h"

class Event;

class EventDAO : public DAO
{
public:
    static EventDAO* getInstance();

    Event* load(int id) const;

private:
    EventDAO();
    void update(Persistable* persistable);
    void insert(Persistable* persistable);
};

#endif // EVENTDAO_H
