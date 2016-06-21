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
    void onThumbnailCreated(const QImage& image);

signals:
    void photoAdded(Photo* photo);

private:
    QList<Photo*>           _photos;
    Library*                _library;
};

#endif // SCANNER_H
