#ifndef PHOTOCLUSTERS_H
#define PHOTOCLUSTERS_H

#include "Geocoder.h"

#include <QDate>
#include <QList>
#include <QMap>

class Photo;
typedef QList<Photo*> PhotoList;

/**
 * A Cluster stores all Photo* in the same day and at the same location
 */
class Cluster
{
public:
    void addPhoto   (Photo* photo);
    void removePhoto(Photo* photo);
    bool colocatedWith(const Photo* photo) const;
    void clear();

    QString     getAddress()    const;
    QDate       getDate()       const;
    QString     getTitle()      const;
    PhotoList   getAllPhotos()  const;
    int         getPhotoCount() const;

private:
    QMap<QDateTime, Photo*> _photos;
};

typedef QList<Cluster*> ClusterList;

/**
 * A SameDateClusters stores clusters within the same day but at different locations
 */
class SameDateClusters
{
public:
    void addCluster(Cluster* cluster);
    Cluster* addPhoto   (Photo* photo);
    void     removePhoto(Photo* photo);
    void clear();

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

/**
 * PhotoClusters stores all the SameDateClusters
 */
class PhotoClusters
{
public:
    Cluster* addPhoto   (Photo* photo);
    Cluster* removePhoto(Photo* photo);
    void clear();

    PhotoList   getAllPhotos()      const;
    ClusterList getAllClusters()    const;

private:
    ClusterLists    _clusterLists;
};

#endif // PHOTOCLUSTERS_H
