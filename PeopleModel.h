#ifndef PEOPLEMODEL_H
#define PEOPLEMODEL_H

#include "ItemModel.h"

class PeopleModel : public ItemModel
{
public:
    explicit PeopleModel(QObject* parent = 0);

    int rowCount    (const QModelIndex& parent = QModelIndex()) const;
    int columnCount (const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif // PEOPLEMODEL_H
