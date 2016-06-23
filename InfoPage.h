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

signals:
    void infoChanged(Photo*);

private:
    Ui::InfoPage ui;
    InfoModel*  _model;
    Photo*      _photo;
};

#endif // INFOPAGE_H
