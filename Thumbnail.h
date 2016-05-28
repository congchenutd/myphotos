#ifndef THUMBNAIL_H
#define THUMBNAIL_H

#include "Persistable.h"

#include <QPixmap>
#include <QThread>
#include <QObject>

class Photo;

class Thumbnail : public Persistable
{
public:
    Thumbnail(int id, const QString& filePath);

    QString getFilePath() const;
    void setFilePath(const QString& path);

private:
    QString _filePath;
};

class ThumbnailThread: public QThread
{
    Q_OBJECT

public:
    ThumbnailThread(Photo* photo);
    void run();

private slots:
    void onFinished();

signals:
    void finished(Photo* photo);

private:
    QString createThumbnailFileName(const QString& filePath);

private:
    Photo*  _photo;
    QString _filePath;
};


#endif // THUMBNAIL_H
