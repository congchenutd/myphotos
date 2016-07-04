#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <functional>

class QLayoutItem;

// Comparators for sorting
typedef std::function<bool (QLayoutItem*, QLayoutItem*)> Comparator;

class ClusterViewLessAddress
{
public:
    ClusterViewLessAddress(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class ClusterViewLessDate
{
public:
    ClusterViewLessDate(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class PhotoItemLessTime
{
public:
    PhotoItemLessTime(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class PhotoItemLessTitle
{
public:
    PhotoItemLessTitle(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

#endif // COMPARATOR_H
