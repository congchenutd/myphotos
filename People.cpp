#include "People.h"
#include "PeopleDAO.h"

People::People(int id, const QString& name)
    : Persistable(id, PeopleDAO::getInstance()), _name(name)
{}

QString People::getName() const {
    return _name;
}

void People::setName(const QString& name) {
    _name = name;
}
