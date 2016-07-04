#ifndef SORTABLEVBOXLAYOUT_H
#define SORTABLEVBOXLAYOUT_H

#include "Comparator.h"
#include <QVBoxLayout>
#include <QList>

class SortableVBoxLayout : public QVBoxLayout
{
public:
    SortableVBoxLayout(QWidget* parent = 0);
    void clear();
    void sort(const Comparator& comparator);
};


#endif // SORTABLEVBOXLAYOUT_H
