#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <functional>
#include <QList>
#include <QLayoutItem>

typedef std::function<bool (QLayoutItem*, QLayoutItem*)> Comparator;

template <typename T>
int partition(QList<T>& list, int left, int right, const Comparator& comparator)
{
    T pivot = list.at((left + right) / 2);
    while (left < right)
    {
        while (left < right && comparator(list.at(left), pivot))
            ++ left;
        while (left < right && !comparator(list.at(right), pivot))
            -- right;
        if (left < right)
        {
            T temp = list.at(left);
            list.replace(left, list.at(right));
            list.replace(right, temp);
        }
    }
    return left;
}

template <typename T>
void quickSort(QList<T>& list, int left, int right, const Comparator& comparator)
{
    if (left >= right)
        return;

    int pivot = partition(list, left, right, comparator);
    quickSort(list, left, pivot - 1, comparator);
    quickSort(list, pivot + 1, right, comparator);
}

#endif // QUICKSORT_H
