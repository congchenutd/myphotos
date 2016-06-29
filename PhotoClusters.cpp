#include "Photo.h"
#include "PhotoClusters.h"
#include <climits>

void Cluster::addPhoto(Photo* photo)
{
    if (photo == 0)
        return;
    _photos.insert(photo->getTimeTaken(), photo);
    photo->setCluster(this);
}

bool Cluster::colocatedWith(const Photo* photo) const {
    return _photos.isEmpty() ? true
                             : _photos.first()->colocatedWith(photo);
}

QString Cluster::getAddress() const {
    return _photos.isEmpty() ? QString()
                             : _photos.first()->getAddress();
}

QDate Cluster::getDate() const {
    return _photos.isEmpty() ? QDate()
                             : _photos.first()->getTimeTaken().date();
}

QString Cluster::getTitle() const {
    return getAddress() + "  " + getDate().toString("MM/dd/yyyy");
}

PhotoList Cluster::getAllPhotos() const {
    return _photos.values();
}


////////////////////////////////////////////////////////////////////////////////////////
void SameDateClusters::addCluster(Cluster* cluster) {
    _clusters << cluster;
}

Cluster* SameDateClusters::addPhoto(Photo* photo)
{
    Cluster* cluster = findCluster(photo);
    if (cluster == 0)
    {
        cluster = new Cluster;
        addCluster(cluster);
    }
    cluster->addPhoto(photo);
    return cluster;
}

ClusterList SameDateClusters::getAllClusters() const {
    return _clusters;
}

Cluster* SameDateClusters::findCluster(Photo* photo) const
{
    foreach (Cluster* cluster, _clusters)
        if (cluster->colocatedWith(photo))
            return cluster;
    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////
Cluster* PhotoClusters::addPhoto(Photo* photo)
{
    QDate date = photo->getTimeTaken().date();
    if (!_clusterLists.contains(date))
        _clusterLists.insert(date, new SameDateClusters());
    return _clusterLists[date]->addPhoto(photo);
}

ClusterList PhotoClusters::getAllClusters() const
{
    ClusterList result;
    foreach (SameDateClusters* sameDate, _clusterLists)
        result << sameDate->getAllClusters();
    return result;
}

