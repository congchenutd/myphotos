#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QRunnable>
#include <QImage>
#include <QSize>

class Photo;
class QVideoFrame;

class ThumbnailGenerator: public QObject, public QRunnable
{
    Q_OBJECT

public:
    ThumbnailGenerator(Photo* photo, const QSize& size);
    Photo* getPhoto() const;

    static ThumbnailGenerator* getGenerator(Photo* photo, const QSize& size);

signals:
    void finished(const QImage& image);

protected:
    Photo*  _photo;
    QSize   _size;
};

class ImageThumbnailGenerator: public ThumbnailGenerator
{
public:
    ImageThumbnailGenerator(Photo* photo, const QSize& size);
    void run();
};

class VideoThumbnailGenerator: public ThumbnailGenerator
{
public:
    VideoThumbnailGenerator(Photo* photo, const QSize& size);
    void run();

private:

};

#endif // THUMBNAILGENERATOR_H
