#include "ItemModel.h"
#include "Library.h"

ItemModel::ItemModel(QObject* parent) : QAbstractItemModel(parent)
{
    _library = Library::getInstance();
}

QModelIndex ItemModel::index(int row, int column, const QModelIndex& parent) const {
    Q_UNUSED(parent)
    return createIndex(row, column);
}

QModelIndex ItemModel::parent(const QModelIndex& index) const {
    Q_UNUSED(index)
    return QModelIndex();
}

void ItemModel::update() {
    endResetModel();
}
