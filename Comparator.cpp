#include "ClusterView.h"
#include "Comparator.h"
#include "PhotoItem.h"
#include "Photo.h"
#include <QLayoutItem>

/////////////////////////////////////////////////////////////////////////////////////////
CompareClusterViewsByDate::CompareClusterViewsByDate(bool lessThan) : _lessThan(lessThan) {}

bool CompareClusterViewsByDate::operator()(QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
        return _lessThan ? view1->getDate() < view2->getDate()
                         : view1->getDate() > view2->getDate();
    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////
CompareClusterViewsByAddress::CompareClusterViewsByAddress(bool lessThan) : _lessThan(lessThan) {}

bool CompareClusterViewsByAddress::operator()(QLayoutItem* lhs, QLayoutItem* rhs) const
{
    ClusterView* view1 = dynamic_cast<ClusterView*>(lhs->widget());
    ClusterView* view2 = dynamic_cast<ClusterView*>(rhs->widget());
    if (view1 != 0 && view2 != 0)
        return _lessThan ? view1->getAddress() < view2->getAddress()
                         : view1->getAddress() > view2->getAddress();
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
ComparePhotoItemsByTime::ComparePhotoItemsByTime(bool lessThan) : _lessThan(lessThan) {}

bool ComparePhotoItemsByTime::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
        return _lessThan ? item1->getPhoto()->getTimeTaken() < item2->getPhoto()->getTimeTaken()
                         : item1->getPhoto()->getTimeTaken() > item2->getPhoto()->getTimeTaken();
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
ComparePhotoItemsByTitle::ComparePhotoItemsByTitle(bool lessThan) : _lessThan(lessThan) {}

bool ComparePhotoItemsByTitle::operator() (QLayoutItem* lhs, QLayoutItem* rhs) const
{
    PhotoItem* item1 = dynamic_cast<PhotoItem*>(lhs->widget());
    PhotoItem* item2 = dynamic_cast<PhotoItem*>(rhs->widget());
    if (item1 != 0 && item2 != 0)
        return _lessThan ? item1->getPhoto()->getTitle() < item2->getPhoto()->getTitle()
                         : item1->getPhoto()->getTitle() > item2->getPhoto()->getTitle();
    return false;
}


bool ComparePhotosByTime::operator() (Photo* lhs, Photo* rhs) const {
    return lhs->getTimeTaken() < rhs->getTimeTaken();
}

bool ComparePhotoItemsByTime2::operator() (PhotoItem *lhs, PhotoItem *rhs) const {
    return lhs->getPhoto()->getTimeTaken() < rhs->getPhoto()->getTimeTaken();
}
