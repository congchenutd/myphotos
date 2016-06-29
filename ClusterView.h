#ifndef CLUSTERVIEW_H
#define CLUSTERVIEW_H

#include <QWidget>
#include "PhotoClusters.h"

class QLabel;
class FlowLayout;
class QLayoutItem;

class ClusterView : public QWidget
{
public:
    explicit ClusterView(Cluster* cluster, QWidget* parent = 0);
    void addPhoto(Photo* photo);
    void reloadTitle();
    QString getTitle()  const;
    QDate   getDate()   const;
    void sort(std::function<bool (QLayoutItem*, QLayoutItem*)> comparator);

private:
    QLabel*     _labelTitle;
    FlowLayout* _flowLayout;
    Cluster*    _cluster;
};

#endif // CLUSTERVIEW_H
