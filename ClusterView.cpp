#include "ClusterView.h"
#include "FlowLayout.h"
#include "PhotoItem.h"

#include <QLabel>

ClusterView::ClusterView(Cluster* cluster, QWidget* parent)
    : QWidget(parent),
      _cluster(cluster)
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    _labelTitle = new QLabel(_cluster->getTitle());
    vBoxLayout->addWidget(_labelTitle);

    _layout = new FlowLayout;
    vBoxLayout->addLayout(_layout);
    vBoxLayout->setAlignment(_layout, Qt::AlignLeft);

    foreach (Photo* photo, _cluster->getAllPhotos())
        addPhoto(photo);
}

void ClusterView::addPhoto(Photo* photo)
{
    PhotoItem* photoItem = new PhotoItem(photo, this);
    connect(photoItem, SIGNAL(titleChanged(QString)), this, SLOT(sort()));
    _layout->addWidget(photoItem);
}

void ClusterView::removePhotoItem(PhotoItem* item)
{
    _layout->removeWidget(item);
    delete item;
}

void ClusterView::reloadTitle() {
    _labelTitle->setText(_cluster->getTitle());
}

int ClusterView::getPhotoItemCount() const {
    return _layout->count();
}

QString ClusterView::getTitle() const {
    return _cluster->getTitle();
}

QDate ClusterView::getDate() const {
    return _cluster->getDate();
}

Cluster* ClusterView::getCluster() const {
    return _cluster;
}

QList<PhotoItem*> ClusterView::getAllPhotoItems() const
{
    QList<PhotoItem*> result;
    for (int i = 0; i < getPhotoItemCount(); ++i)
        result << static_cast<PhotoItem*>(_layout->itemAt(i)->widget());
    return result;
}

PhotoItem* ClusterView::findPhotoItem(const Photo* photo) const
{
    foreach (PhotoItem* item, getAllPhotoItems())
        if (item->getPhoto() == photo)
            return item;
    return 0;
}

void ClusterView::sort(std::function<bool (QLayoutItem *, QLayoutItem *)> comparator)
{
    _comparator = comparator;
    _layout->sort(comparator);
}

void ClusterView::sort() {
    sort(_comparator);
}
