#ifndef ITEMMODEL_H
#define ITEMMODEL_H

#include <QDate>
#include <QAbstractItemModel>

class Library;

class ItemModel : public QAbstractItemModel
{
public:
    explicit ItemModel(QObject* parent = 0);

    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
    void update();

protected:
    Library* _library;
};

#endif // ITEMMODEL_H
