#include "ClusterView.h"
#include "Comparator.h"
#include "PhotoItem.h"
#include "Photo.h"
#include <QLayoutItem>

/////////////////////////////////////////////////////////////////////////////////////////
ClusterViewLessDate::ClusterViewLessDate(bool lessThan) : _lessThan(lessThan) {}

bool ClusterViewLessDate::operator()(QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
        return _lessThan ? view1->getDate() < view2->getDate()
                         : view1->getDate() > view2->getDate();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
ClusterViewLessAddress::ClusterViewLessAddress(bool lessThan) : _lessThan(lessThan) {}

bool ClusterViewLessAddress::operator()(QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
        return _lessThan ? view1->getAddress() < view2->getAddress()
                         : view1->getAddress() > view2->getAddress();
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
PhotoItemLessTime::PhotoItemLessTime(bool lessThan) : _lessThan(lessThan) {}

bool PhotoItemLessTime::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
        return _lessThan ? item1->getPhoto()->getTimeTaken() < item2->getPhoto()->getTimeTaken()
                         : item1->getPhoto()->getTimeTaken() > item2->getPhoto()->getTimeTaken();
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
PhotoItemLessTitle::PhotoItemLessTitle(bool lessThan) : _lessThan(lessThan) {}

bool PhotoItemLessTitle::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
        return _lessThan ? item1->getPhoto()->getTitle() < item2->getPhoto()->getTitle()
                         : item1->getPhoto()->getTitle() > item2->getPhoto()->getTitle();
    return false;
}

