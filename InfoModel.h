#ifndef INFOMODEL_H
#define INFOMODEL_H

#include <QStandardItemModel>

class Photo;

class InfoModel : public QStandardItemModel
{
public:
    InfoModel(QObject* parent = 0);
    void setPhoto(Photo* photo);
    void save();

private:
    Photo* _photo;

    enum {ROW_TITLE, ROW_PATH, ROW_TIME};
    enum {COL_PROPERTY, COL_VALUE};
};

#endif // INFOMODEL_H
