#ifndef SCANNER_H
#define SCANNER_H

#include <QFileInfo>
#include <QFutureWatcher>
#include <QImage>
#include <QObject>
#include <QQueue>
#include <QSize>
#include <QList>

class Photo;
class Library;
class Thumbnail;
class PhotoInfo;

/**
 * A Scanner scans monitored folders and create Photos and their thumbnails
 */
class Scanner: public QObject
{
    Q_OBJECT

public:
    Scanner();
    int scan();

private slots:
    void onThumbnailCreated     (int index);
    void onPhotoInfoExtracted   (int index);

signals:
    void photoAdded(Photo* photo);

private:
    QList<Photo*>           _photos;
    Library*                _library;
    QFutureWatcher<Thumbnail*>* _thumbnailFutureWatcher;
    QFutureWatcher<PhotoInfo*>* _infoFutureWatcher;
};

#endif // SCANNER_H
