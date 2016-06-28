#ifndef PHOTOVIEW2_H
#define PHOTOVIEW2_H

#include <QWidget>

class QLayout;
class ClusterView;

class PhotoView2 : public QWidget
{
    Q_OBJECT
public:
    explicit PhotoView2(QWidget* parent = 0);

signals:

public slots:

private:
    QLayout*    _vBoxLayout;
};


#endif // PHOTOVIEW2_H
