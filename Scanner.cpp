#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoDAO.h"
#include "ThumbnailDAO.h"
#include "ThumbnailGenerator.h"
#include "Exif.h"
#include "Geocoder.h"
#include <QDir>
#include <QImageReader>
#include <QtConcurrent>
#include <QThreadPool>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>

/**
 * Scan monitored folders for photos
 * @return the photos
 */
QList<Photo*> Scanner::scan()
{
    QList<Photo*> photos;

    // Collect photos
    QStringList folders = Settings::getInstance()->getMonitoredFolders();
    for (const QString& folder: folders)
    {
        QDir dir(folder);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setSorting(QDir::Time | QDir::Reversed);
        dir.setNameFilters(Settings::getInstance()->getMonitoredFileTypes().split(";"));
        // for each photo file
        for (const QFileInfo& info: dir.entryInfoList())
            if (Library::getInstance()->getPhoto(info.filePath()) == 0) // doesn't exist
            {
                // create a photo object
                Photo* photo = new Photo(PhotoDAO::getInstance()->getNextID(),
                                         info.baseName(),
                                         info.filePath(),
                                         info.lastModified());
                photo->save();  // for getNextID() to work correctly
                photos << photo;
            }
    }

    // assign the photos to ScannerThread for generating thumbnail and exif
    ScannerThread* thread = new ScannerThread(QList<Photo*>(photos));
    connect(thread, SIGNAL(scanned(Photo*)), SIGNAL(scanned(Photo*)));
    QThreadPool::globalInstance()->start(thread);

    return photos;
}

///////////////////////////////////////////////////////////////////////////////////////////
ScannerThread::ScannerThread(const QList<Photo*>& photos)
    : _photos(photos) {}

void ScannerThread::run()
{
    Library* library = Library::getInstance();
    for (Photo* photo: _photos)
    {
        Thumbnail* thumbnail = generateThumbnail(photo);
        photo->setThumbnail(thumbnail);
        library->addPhoto(photo);
        library->addThumbnail(thumbnail);
        thumbnail->save();

        Exif exif(photo);
        photo->setExif(exif);
        photo->save();
        emit scanned(photo);
    }
}
