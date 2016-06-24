#ifndef PHOTOINFO_H
#define PHOTOINFO_H

#include <QMap>
#include <QString>

class Photo;

class PhotoInfo
{
public:
    PhotoInfo(const QString& json = QString());
    PhotoInfo(Photo* photo);
    QString toJson() const;
    QMap<QString, QString> getData() const;
    QString getValue(const QString& property) const;
    void setValue(const QString& property, const QString& value);

private:
    QMap<QString, QString>  _data;
};

///////////////////////////////////////////////////////////////////////
PhotoInfo* extractPhotoInfo(Photo* photo);

#endif // PHOTOINFO_H
