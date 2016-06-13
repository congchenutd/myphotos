#ifndef TAGSMODEL_H
#define TAGSMODEL_H

#include "ItemModel.h"

class TagModel : public ItemModel
{
public:
    explicit TagModel(QObject* parent = 0);

    int rowCount    (const QModelIndex& parent = QModelIndex()) const;
    int columnCount (const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
};

#endif // TAGSMODEL_H
