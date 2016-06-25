#include "Geocoder.h"
#include "Photo.h"
#include "Exif.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

Exif::Exif(const QString& json)
{
    if (json.isEmpty())
        return;

    QJsonObject jsonObj = QJsonDocument::fromJson(json.toUtf8()).object();
    for (QJsonObject::iterator it = jsonObj.begin(); it != jsonObj.end(); ++it)
    {
        QString property    = it.key();
        QString value       = it.value().toString();
        _data.insert(property, value);
    }
}

Exif::Exif(Photo* photo)
{
    if (photo == 0)
        return;

    QProcess* process = new QProcess;
    process->start("/usr/local/bin/exiftool", QStringList() << photo->getFilePath());
    process->waitForFinished();

    QStringList list = QString(process->readAllStandardOutput()).split("\n");
    foreach (QString line, list)
    {
        int indexColon = line.indexOf(':');
        if (indexColon > 0)
        {
            QString property    = line.left(indexColon).simplified();
            QString value       = line.right(line.length() - indexColon - 1).simplified();
            _data.insert(property, value);
        }
    }
}

QString Exif::toJson() const
{
    QJsonObject jsonObj;
    for (QMap<QString, QString>::const_iterator it = _data.begin(); it != _data.end(); ++it)
    {
        QString k = it.key();
        QString v = it.value();
        jsonObj.insert(it.key(), it.value());
    }
    QString s = QJsonDocument(jsonObj).toJson();
    return QJsonDocument(jsonObj).toJson();
}

QMap<QString, QString> Exif::getData() const {
    return _data;
}

QString Exif::getValue(const QString& property) const {
    return _data.contains(property) ? _data[property] : QString();
}

void Exif::setValue(const QString& property, const QString& value) {
    _data[property] = value;
}

//void Exif::updateLocation()
//{
//    QString latitude    = getValue("GPS Latitude");
//    QString longitude   = getValue("GPS Longitude");
//    if (latitude.isEmpty() || longitude.isEmpty())
//        return;

//    Geocoder* geocoder = new Geocoder;
//    geocoder->queryLocation(latitude, longitude);
//}

////////////////////////////////////////////////////////////////////////////////
Exif extracExif(Photo* photo) {
    return Exif(photo);
}
