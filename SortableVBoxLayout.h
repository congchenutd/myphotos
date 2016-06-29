#ifndef SORTABLEVBOXLAYOUT_H
#define SORTABLEVBOXLAYOUT_H

#include <QVBoxLayout>

class SortableVBoxLayout : public QVBoxLayout
{
public:
    SortableVBoxLayout(QWidget* parent = 0);

    void sort(std::function<bool (QLayoutItem*, QLayoutItem*)> comparator);
};

#endif // SORTABLEVBOXLAYOUT_H
