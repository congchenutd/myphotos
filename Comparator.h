#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <functional>

class QLayoutItem;
class Photo;
class PhotoItem;

// Comparators for sorting
typedef std::function<bool (QLayoutItem*, QLayoutItem*)> LayoutItemComparator;

class CompareClusterViewsByAddress
{
public:
    CompareClusterViewsByAddress(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class CompareClusterViewsByDate
{
public:
    CompareClusterViewsByDate(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class ComparePhotoItemsByTime
{
public:
    ComparePhotoItemsByTime(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;

private:
    bool _lessThan;
};

class ComparePhotoItemsByTitle
{
public:
    ComparePhotoItemsByTitle(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class ComparePhotoItemsByTime2
{
public:
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;
};

#endif // COMPARATOR_H
