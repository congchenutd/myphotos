#include "Clustering.h"
#include "Photo.h"

#include <QDate>
#include <QMap>

//QList<Cluster> Clustering::run(const QList<Photo*>& photos)
//{
//    QMap<QDate, Cluster> date2Cluster;
//    foreach (Photo* photo, photos)
//        date2Cluster[photo->getTimeTaken().date()] << photo;

//    QList<Cluster> clusters;
//    foreach (Cluster cluster, date2Cluster)
//        clusters << split(cluster);

//    return clusters;
//}

//QList<Cluster> Clustering::split(const Cluster& cluster)
//{
//    QMap<QString, Cluster> clusters;
//    for (Photo* photo: cluster)
//        clusters[photo->getLocation().toString()] << photo;
//    return clusters.values();
//}
