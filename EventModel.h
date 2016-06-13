#ifndef EVENTSMODEL_H
#define EVENTSMODEL_H

#include "ItemModel.h"

class EventModel : public ItemModel
{
    Q_OBJECT

public:
    explicit EventModel(QObject* parent = 0);

    int rowCount    (const QModelIndex& parent = QModelIndex()) const;
    int columnCount (const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    bool setData(const QModelIndex& idx, const QVariant &value, int role = Qt::EditRole);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

    void update();

public:
    enum {COL_NAME, COL_DATE};
};

#endif // EVENTSMODEL_H
