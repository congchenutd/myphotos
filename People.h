#ifndef PEOPLE_H
#define PEOPLE_H

#include <QString>
#include "Persistable.h"

class PeopleDAO;

class People: public Persistable
{
public:
    People(int id, const QString& name);

    QString getName() const;
    void setName(const QString& name);

private:
    QString _name;
};

#endif // PEOPLE_H
