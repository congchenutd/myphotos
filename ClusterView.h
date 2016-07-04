#ifndef CLUSTERVIEW_H
#define CLUSTERVIEW_H

#include <QWidget>
#include "PhotoClusters.h"
#include "Comparator.h"

class QLabel;
class FlowLayout;
class QLayoutItem;
class PhotoItem;

/**
 * A ClusterView holds PhotoItems that have the same date and location
 */
class ClusterView : public QWidget
{
public:
    explicit ClusterView(Cluster* cluster, QWidget* parent = 0);
    void addPhoto(Photo* photo);
    void removePhotoItem(PhotoItem* item);
    void reloadTitle();
    void sort(const Comparator& comparator);

    int                 getPhotoItemCount() const;
    QString             getTitle()          const;
    QDate               getDate()           const;
    QString             getAddress()        const;
    Cluster*            getCluster()        const;
    QList<PhotoItem*>   getAllPhotoItems()  const;

private:
    QLabel*     _labelTitle;
    FlowLayout* _layout;
    Cluster*    _cluster;
};

#endif // CLUSTERVIEW_H
