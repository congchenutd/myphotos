#ifndef EXIFVIEW_H
#define EXIFVIEW_H

#include "ui_ExifView.h"

class Photo;

class ExifView : public QWidget
{
    Q_OBJECT

public:
    explicit ExifView(QWidget* parent = 0);

private slots:
    void onPhotoSelected(Photo* photo);

private:
    Ui::ExifView ui;
    Photo* _photo;
};

#endif // EXIFVIEW_H
