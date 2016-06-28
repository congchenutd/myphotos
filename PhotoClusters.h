#ifndef PHOTOCLUSTERS_H
#define PHOTOCLUSTERS_H

#include <QDate>
#include <QList>
#include <QMap>

class Photo;

typedef QMap<QDateTime, Photo*>         Cluster;
typedef QMap<QString, Cluster>          Location2Cluster;
typedef QMap<QDate, Location2Cluster>   Date2Clusters;

class PhotoClusters
{
public:
    PhotoClusters();
    void addPhoto(Photo* photo);

private:
    Date2Clusters   _date2Clusters;
    QList<Photo*>   _photos;
};

#endif // PHOTOCLUSTERS_H
