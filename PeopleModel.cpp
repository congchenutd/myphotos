#include "PeopleModel.h"
#include "Library.h"

PeopleModel::PeopleModel(QObject* parent)
    : ItemModel(parent) {}

int PeopleModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return _library->getAllPeople().count();
}

int PeopleModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return 1;
}

QVariant PeopleModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return _library->getAllPeople().keys().at(index.row());

    return QVariant();
}
