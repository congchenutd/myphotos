#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QRunnable>
#include <QList>

class Photo;
class Library;
class Thumbnail;
class Exif;
class Geocoder;

/**
 * A Scanner scans monitored folders and create Photos and their thumbnails
 */
class Scanner: public QObject
{
    Q_OBJECT

public:
    QList<Photo*> scan();

signals:
    void scanned(Photo* photo);
};

class ScannerThread: public QObject, public QRunnable
{
    Q_OBJECT

public:
    ScannerThread(const QList<Photo*>& photos);
    void run();

signals:
    void scanned(Photo* photo);

private:
    QList<Photo*>   _photos;
};

#endif // SCANNER_H
