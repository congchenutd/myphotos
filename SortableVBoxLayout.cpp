#include "ClusterView.h"
#include "SortableVBoxLayout.h"
#include <QLabel>

SortableVBoxLayout::SortableVBoxLayout(QWidget* parent)
    : QVBoxLayout(parent) {}

void SortableVBoxLayout::clear() {
    while (QLayoutItem* item = takeAt(0))
        item->widget()->deleteLater();
}

void SortableVBoxLayout::sort(const LayoutItemComparator& comparator)
{
    QList<QLayoutItem*> items;
    while (QLayoutItem* item = takeAt(0))
        items << item;

    std::sort(items.begin(), items.end(), comparator);

    foreach (QLayoutItem* item, items)
        addItem(item);
}
