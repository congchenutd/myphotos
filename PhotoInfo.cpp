#include "Photo.h"
#include "PhotoInfo.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QProcess>

PhotoInfo::PhotoInfo(const QString& json)
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

PhotoInfo::PhotoInfo(Photo* photo)
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

QString PhotoInfo::toJson() const
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

QMap<QString, QString> PhotoInfo::getData() const {
    return _data;
}

QString PhotoInfo::getValue(const QString& property) const {
    return _data.contains(property) ? _data[property] : QString();
}

void PhotoInfo::setValue(const QString& property, const QString& value) {
    _data[property] = value;
}

////////////////////////////////////////////////////////////////////////////////
PhotoInfo* extractPhotoInfo(Photo* photo) {
    return new PhotoInfo(photo);
}
