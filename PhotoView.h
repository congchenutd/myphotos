#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include "ui_PhotoView.h"

class Library;
class Photo;

class PhotoView : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoView(QWidget* parent = 0);

    void load();

public slots:
    void addPhoto(Photo* photo);

private:
    Ui::PhotoView ui;
    QLayout* _layout;
    Library* _library;
};

#endif // PHOTOVIEW_H
