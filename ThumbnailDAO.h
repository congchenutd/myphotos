#ifndef THUMBNAILDAO_H
#define THUMBNAILDAO_H

#include "DAO.h"
#include "Thumbnail.h"

class ThumbnailDAO : public DAO
{
public:
public:
    static ThumbnailDAO* getInstance();

    Thumbnail* load(int id) const;
    void remove(Persistable* persistable);

private:
    ThumbnailDAO();
    void update(Persistable* persistable);
    void insert(Persistable* persistable);
};

#endif // THUMBNAILDAO_H
