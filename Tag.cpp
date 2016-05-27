#include "Tag.h"
#include "TagDAO.h"

Tag::Tag(int id, const QString& name)
    : Persistable(id, TagDAO::getInstance()),
      _name(name)
{}

QString Tag::getName() const {
    return _name;
}

void Tag::setName(const QString& name) {
    _name = name;
}


