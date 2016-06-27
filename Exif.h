#ifndef PHOTOINFO_H
#define PHOTOINFO_H

#include <QMap>
#include <QString>

class Photo;

class Exif
{
public:
    Exif(const QString& json = QString());
    Exif(Photo* photo);
    QString toJson() const;
    QMap<QString, QString> getData() const;
    QString getValue(const QString& property) const;
    void setValue(const QString& property, const QString& value);

private:
    QMap<QString, QString>  _data;
    QString _address1;
    QString _address2;
};

///////////////////////////////////////////////////////////////////////
Exif extracExif(Photo* photo);

#endif // PHOTOINFO_H
