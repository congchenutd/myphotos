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

int Cluster::getPhotoCount() const {
    return _photos.count();
}

void Cluster::removePhoto(Photo* photo)
{
    for (QMap<QDateTime, Photo*>::iterator it = _photos.begin(); it != _photos.end(); ++it)
        if (it.value() == photo)
            _photos.erase(it);
}


////////////////////////////////////////////////////////////////////////////////////////
void SameDateClusters::addCluster(Cluster* cluster) {
    _clusters << cluster;
}

Cluster* SameDateClusters::addPhoto(Photo* photo)
{
    Cluster* cluster = findColocatedCluster(photo);
    if (cluster == 0)
    {
        cluster = new Cluster;
        addCluster(cluster);
    }
    cluster->addPhoto(photo);
    _date = photo->getTimeTaken().date();
    return cluster;
}

void SameDateClusters::removePhoto(Photo* photo)
{
    if (Cluster* cluster = findColocatedCluster(photo))
    {
        cluster->removePhoto(photo);
        if (cluster->getPhotoCount() == 0)
        {
            _clusters.removeAll(cluster);
            delete cluster;
        }
    }
}

QDate SameDateClusters::getDate() const {
    return _date;
}

ClusterList SameDateClusters::getAllClusters() const {
    return _clusters;
}

int SameDateClusters::getClusterCount() const {
    return _clusters.count();
}

Cluster* SameDateClusters::findColocatedCluster(Photo* photo) const
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

Cluster* PhotoClusters::removePhoto(Photo* photo)
{
    QDate date = photo->getTimeTaken().date();
    if (_clusterLists.contains(date))
    {
        SameDateClusters* sameDate = _clusterLists[date];
        sameDate->removePhoto(photo);
        if (sameDate->getClusterCount() == 0)
        {
            _clusterLists.remove(sameDate->getDate());
            delete sameDate;
        }
    }
    return 0;
}

ClusterList PhotoClusters::getAllClusters() const
{
    ClusterList result;
    foreach (SameDateClusters* sameDate, _clusterLists)
        result << sameDate->getAllClusters();
    return result;
}

