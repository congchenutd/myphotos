#include "EventModel.h"
#include "Library.h"
#include "Event.h"
#include <QAbstractItemModel>

EventModel::EventModel(QObject* parent)
    : ItemModel(parent)
{}

int EventModel::rowCount(const QModelIndex&) const {
    return _library->getAllEvents().count();
}

int EventModel::columnCount(const QModelIndex&) const {
    return 2;
}

QVariant EventModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        QString eventName = _library->getAllEvents().keys().at(index.row());
        Event* event = _library->getEvent(eventName);
        if (index.column() == COL_NAME)
            return event->getName();
        if (index.column() == COL_DATE)
            return event->getDate();
    }

    return QVariant();
}

bool EventModel::setData(const QModelIndex& idx, const QVariant& value, int role)
{
    if (role != Qt::EditRole)
        return false;

    QString eventName = data(index(idx.row(), COL_NAME)).toString();
    Event* event = _library->getEvent(eventName);
    if (event == 0)
        return false;

    if (idx.column() == COL_NAME)
        event->setName(value.toString());
    else if (idx.column() == COL_DATE)
        event->setDate(value.toDate());
    emit dataChanged(idx, idx);
    return true;
}

QVariant EventModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == COL_NAME)
            return tr("Name");
        if (section == COL_DATE)
            return tr("Date");
    }

    return QVariant();
}

Qt::ItemFlags EventModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    // make it editable
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}
