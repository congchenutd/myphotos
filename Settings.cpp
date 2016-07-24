#include "Settings.h"

#include <QDir>
#include <QResource>
#include <QSize>

Settings *Settings::getInstance()
{
    if (_instance == 0)
        _instance = new Settings("MyPhotos.ini");
    return _instance;
}

QStringList Settings::getMonitoredFolders() const {
    return value("MonitoredFolders").toStringList();
}

void Settings::setMonitoredFolders(const QStringList& list) {
    setValue("MonitoredFolders", list);
}

QString Settings::getMonitoredFileTypes() const {
    return value("MonitoredFileTypes").toString();
}

void Settings::setMonitoredFileTypes(const QString& list)
{
    QStringList unsorted = list.split(";");
    unsorted.sort();
    setValue("MonitoredFileTypes", unsorted.join(";"));
}

QSize Settings::getNewThumbnailSize() const {
    return QSize(500, 500);
}

QSize Settings::getThumbnailSize() const {
    return value("ThumbnailSize").toSize();
}

void Settings::setThumbnailSize(const QSize& size) {
    setValue("ThumbnailSize", size);
}

Settings::Settings(const QString& fileName)
    : QSettings(fileName, QSettings::IniFormat) {}

Settings* Settings::_instance = 0;

QString Settings::getThumbnailCacheLocation() const
{
    QDir dir = QDir::current();
    dir.mkdir("Thumbnails");
    dir.cd("Thumbnails");
    return dir.path();
}

QString Settings::getTrashLocation() const {
    QDir::current().mkdir("Trash");
    return QString("./Trash");
}

QString Settings::getCompileDate() const
{
    // CompileData.txt records the date of the build
    QResource resource(":/CompileDate.txt");
    QString result = (char*) resource.data();
    return result.isEmpty() ? "Unknown date" : result;
}

QString Settings::getExiftoolPath() const {
    return value("Exiftool").toString();
}

void Settings::setExiftoolPath(const QString& path) {
    setValue("Exiftool", path);
}

QString Settings::getFfmpegPath() const {
    return value("Ffmpeg").toString();
}

void Settings::setFfmpegPath(const QString& path) {
    setValue("Ffmpeg", path);
}

bool Settings::getShowTitle() const {
    return value("ShowTitle").toBool();
}

void Settings::setShowTitle(bool show) {
    setValue("ShowTitle", show);
}
