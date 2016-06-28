#include "ClusterView.h"
#include "FlowLayout.h"
#include "PhotoItem.h"

#include <QLabel>

//ClusterView::ClusterView(const QString& title, const Cluster& photos, QWidget* parent)
//    : QWidget(parent),
//      _thumbnailSize(100)
//{
//    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
//    setLayout(vBoxLayout);

//    QLabel* label = new QLabel(title);
//    vBoxLayout->addWidget(label);

//    _flowLayout = new FlowLayout;
//    vBoxLayout->addLayout(_flowLayout);
//    vBoxLayout->setAlignment(_flowLayout, Qt::AlignLeft);

//    foreach (Photo* photo, photos)
//        addPhoto(photo, _thumbnailSize);
//}

//void ClusterView::addPhoto(Photo* photo, int thumbnailSize)
//{
//    PhotoItem* photoItem = new PhotoItem(photo);
//    photoItem->resizeThumbnail(200);
////    connect(photoItem, SIGNAL(titleChanged(QString)), this, SLOT(sort()));
//    _flowLayout->addWidget(photoItem);
////    _photos.insert(photo, photoItem);
//}
