#include "ClusterView.h"
#include "FlowLayout.h"
#include "PhotoItem.h"

#include <QLabel>

ClusterView::ClusterView(Cluster* cluster, QWidget* parent)
    : QWidget(parent),
      _cluster(cluster)
{
    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    QFrame* line = new QFrame(this);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    vBoxLayout->addWidget(line);

    _labelTitle = new QLabel(_cluster->getTitle(), this);
    vBoxLayout->addWidget(_labelTitle);

    _layout = new FlowLayout;
    vBoxLayout->addLayout(_layout);
    vBoxLayout->setAlignment(_layout, Qt::AlignLeft);

    foreach (Photo* photo, _cluster->getAllPhotos())
        addPhoto(photo);

    vBoxLayout->addStretch(10);
}

void ClusterView::addPhoto(Photo* photo) {
    _layout->addWidget(new PhotoItem(photo, this));
}

void ClusterView::removePhotoItem(PhotoItem* item)
{
    _layout->removeWidget(item);
    delete item;
}

void ClusterView::reloadTitle() {
    _labelTitle->setText(_cluster->getTitle());
}

int         ClusterView::getPhotoItemCount()    const { return _layout->count();            }
QString     ClusterView::getTitle()             const { return getCluster()->getTitle();    }
QDate       ClusterView::getDate()              const { return getCluster()->getDate();     }
QString     ClusterView::getAddress()           const { return getCluster()->getAddress();  }
Cluster*    ClusterView::getCluster()           const { return _cluster;                    }

QList<PhotoItem*> ClusterView::getAllPhotoItems() const
{
    QList<PhotoItem*> result;
    for (int i = 0; i < getPhotoItemCount(); ++i)
        result << static_cast<PhotoItem*>(_layout->itemAt(i)->widget());
    return result;
}

void ClusterView::sort(const Comparator& comparator) {
    _layout->sort(comparator);
}

