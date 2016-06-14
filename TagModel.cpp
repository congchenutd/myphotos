#include "TagModel.h"
#include "Library.h"

TagModel::TagModel(QObject* parent)
    : ItemModel(parent) {}

int TagModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return _library->getAllTags().count();
}

int TagModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return 1;
}

QVariant TagModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
        return _library->getAllTags().keys().at(index.row());

    return QVariant();
}
