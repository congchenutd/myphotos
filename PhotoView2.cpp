#include "ClusterView.h"
#include "PhotoView2.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>

PhotoView2::PhotoView2(QWidget* parent)
    : QWidget(parent)
{
    _vBoxLayout = new QVBoxLayout(this);
//    _vBoxLayout->addWidget(new ClusterView(this));
//    _vBoxLayout->addWidget(new ClusterView(this));
}

//ClusterView::ClusterView(QWidget* parent)
//    : QWidget(parent)
//{
//    _hBoxLayout = new QHBoxLayout(this);
//    _hBoxLayout->addWidget(new QLabel("Test1"));
//    _hBoxLayout->addWidget(new QLabel("Test2"));
//    _hBoxLayout->addWidget(new QLabel("Test3"));
//}
