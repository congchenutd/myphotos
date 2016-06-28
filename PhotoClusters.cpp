#include "Photo.h"
#include "PhotoClusters.h"

PhotoClusters::PhotoClusters()
{

}

void PhotoClusters::addPhoto(Photo* photo)
{
    _photos << photo;

    QDate date = photo->getTimeTaken().date();
    if (!_date2Clusters.contains(date))
        _date2Clusters.insert(date, Location2Cluster());
    Location2Cluster& location2Cluster = _date2Clusters[date];

    QString location = photo->getAddress().toString();
    if (!location2Cluster.contains(location))
        location2Cluster.insert(location, Cluster());
    Cluster& cluster = location2Cluster[location];

    cluster[photo->getTimeTaken()] = photo;
}
