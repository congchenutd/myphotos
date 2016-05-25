#include "Settings.h"

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

Settings::Settings(const QString& fileName)
    : QSettings(fileName, QSettings::IniFormat) {}

Settings* Settings::_instance = 0;
