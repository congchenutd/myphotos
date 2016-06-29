#ifndef CLUSTERVIEW_H
#define CLUSTERVIEW_H

#include <QWidget>
#include "PhotoClusters.h"

class QLabel;
class FlowLayout;
class QLayoutItem;
class PhotoItem;

class ClusterView : public QWidget
{
    Q_OBJECT

public:
    explicit ClusterView(Cluster* cluster, QWidget* parent = 0);
    void addPhoto(Photo* photo);
    void removePhotoItem(PhotoItem* item);
    void reloadTitle();

    int                 getPhotoItemCount() const;
    QString             getTitle()          const;
    QDate               getDate()           const;
    Cluster*            getCluster()        const;
    QList<PhotoItem*>   getAllPhotoItems()  const;
    PhotoItem*          findPhotoItem(const Photo* photo) const;
    void sort(std::function<bool (QLayoutItem*, QLayoutItem*)> comparator);

private slots:
    void sort();

private:
    QLabel*     _labelTitle;
    FlowLayout* _layout;
    Cluster*    _cluster;
    std::function<bool (QLayoutItem*, QLayoutItem*)> _comparator;
};

#endif // CLUSTERVIEW_H
