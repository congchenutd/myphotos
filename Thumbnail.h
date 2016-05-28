#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include "Persistable.h"

#include <QPixmap>
#include <QThread>

class Photo;

class Thumbnail : public Persistable
{
public:
    Thumbnail(int id, const QString& filePath);

    QString getFilePath() const;
    void setFilePath(const QString& path);

    static Thumbnail* fromPhoto(Photo* photo);

private:
    static QString createThumbnailFileName(const QString &filePath);

private:
    QString _filePath;
};

class ThumbnailThread: public QThread
{
public:
    ThumbnailThread(Photo* photo, const QString& filePath);
    void run();

private:
    Photo*  _photo;
    QString _filePath;
};

#endif // THUMBNAIL_H
