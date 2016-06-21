#ifndef INFOPAGE_H
#define INFOPAGE_H

#include "ui_InfoPage.h"

class Photo;
class InfoModel;
class QProcess;

class InfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit InfoPage(QWidget* parent = 0);
    void setCurrentPhoto(Photo* photo);

private slots:
    void onEditFinished();

private:
    Ui::InfoPage ui;
    InfoModel*  _model;
};

#endif // INFOPAGE_H
