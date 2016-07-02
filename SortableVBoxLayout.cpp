#include "ClusterView.h"
#include "SortableVBoxLayout.h"
#include <QDebug>
#include <QLabel>

SortableVBoxLayout::SortableVBoxLayout(QWidget* parent)
    : QVBoxLayout(parent) {}

void SortableVBoxLayout::clear() {
    while (QLayoutItem* item = takeAt(0))
        item->widget()->deleteLater();
}

void SortableVBoxLayout::sort(const Comparator& comparator)
{
    QList<QLayoutItem*> items;
    while (QLayoutItem* item = takeAt(0))
        items << item;

    quickSort(items, 0, items.length() - 1, comparator);

    foreach (QLayoutItem* item, items)
        addItem(item);
}
