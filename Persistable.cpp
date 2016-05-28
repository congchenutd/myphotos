#include "Persistable.h"
#include "DAO.h"

Persistable::Persistable(int id, DAO* dao)
    : _id(id), _dao(dao)
{
    _maxID = qMax(id, _maxID);
}

int Persistable::getID() const {
    return _id;
}

int Persistable::getNextID() {
    return _maxID + 1;
}

void Persistable::destroy() {
    _dao->remove(this);
}

void Persistable::save() {
    _dao->save(this);
}

int Persistable::_maxID = 0;
