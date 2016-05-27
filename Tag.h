#ifndef TAG_H
#define TAG_H

#include "Persistable.h"
#include <QString>

class Tag : public Persistable
{
public:
    Tag(int id, const QString& name);

    QString getName() const;
    void setName(const QString& name);

private:
    QString _name;
};

#endif // TAG_H
