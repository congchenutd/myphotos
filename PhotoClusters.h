#ifndef PHOTOCLUSTERS_H
#define PHOTOCLUSTERS_H

#include "Geocoder.h"

#include <QDate>
#include <QList>
#include <QMap>

class Photo;
typedef QList<Photo*> PhotoList;

class Cluster
{
public:
    void addPhoto   (Photo* photo);
    void removePhoto(Photo* photo);
    bool colocatedWith(const Photo* photo) const;
    QString     getAddress()    const;
    QDate       getDate()       const;
    QString     getTitle()      const;
    PhotoList   getAllPhotos()  const;
    int         getPhotoCount() const;

private:
    QMap<QDateTime, Photo*> _photos;
};

typedef QList<Cluster*> ClusterList;

class SameDateClusters
{
public:
    void addCluster(Cluster* cluster);
    Cluster* addPhoto   (Photo* photo);
    void     removePhoto(Photo* photo);
    QDate       getDate()           const;
    ClusterList getAllClusters()    const;
    int         getClusterCount()   const;

private:
    Cluster* findColocatedCluster(Photo* photo) const;

private:
    ClusterList _clusters;
    QDate       _date;
};

typedef QMap<QDate, SameDateClusters*>   ClusterLists;

class PhotoClusters
{
public:
    Cluster* addPhoto   (Photo* photo);
    Cluster* removePhoto(Photo* photo);
    PhotoList   getAllPhotos()      const;
    ClusterList getAllClusters()    const;

private:
    ClusterLists    _clusterLists;
};

#endif // PHOTOCLUSTERS_H
