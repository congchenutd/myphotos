#ifndef INFOPAGE_H
#define INFOPAGE_H

#include "ui_InfoPage.h"

class Photo;
class QStandardItemModel;
class QProcess;

class InfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit InfoPage(QWidget* parent = 0);
    void setCurrentPhoto(Photo* photo);

private slots:
    void onReadyRead();
    void onFinished();

private:
    Ui::InfoPage ui;
    QStandardItemModel* _model;
    QProcess*           _process;
    QString             _output;
};

#endif // INFOPAGE_H
