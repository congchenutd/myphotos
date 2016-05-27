#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
    static Settings* getInstance();

    QStringList getMonitoredFolders() const;
    void setMonitoredFolders(const QStringList& list);

    QString getMonitoredFileTypes() const;
    void setMonitoredFileTypes(const QString& list);

private:
    Settings(const QString& fileName);

private:
    static Settings* _instance;
};

#endif // SETTINGS_H
