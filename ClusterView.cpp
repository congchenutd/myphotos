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
    PhotoItem* photoItem = new PhotoItem(photo);
//    connect(photoItem, SIGNAL(titleChanged(QString)), this, SLOT(sort()));
    _layout->addWidget(photoItem);
    //    _photos.insert(photo, photoItem);
}

void ClusterView::reloadTitle() {
    _labelTitle->setText(_cluster->getTitle());
}

QString ClusterView::getTitle() const {
    return _cluster->getTitle();
}

QDate ClusterView::getDate() const {
    return _cluster->getDate();
}

QList<PhotoItem *> ClusterView::getAllPhotoItems() const
{
    QList<PhotoItem*> result;
    for (int i = 0; i < _layout->count(); ++i)
        if (PhotoItem* item = dynamic_cast<PhotoItem*>(_layout->itemAt(i)->widget()))
            result << item;
    return result;
}

void ClusterView::sort(std::function<bool (QLayoutItem *, QLayoutItem *)> comparator) {
    _layout->sort(comparator);
}
