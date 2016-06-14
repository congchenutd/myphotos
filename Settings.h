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

    QSize getNewThumbnailSize() const;
    void setNewThumbnailSize(const QSize& size);

    QSize getThumbnailSize() const;
    void setThumbnailSize(const QSize& size);

    QString getThumbnailCacheLocation() const;
    QString getTrashLocation() const;

private:
    Settings(const QString& fileName);

private:
    static Settings* _instance;
};

#endif // SETTINGS_H
