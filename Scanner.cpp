#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "ThumbnailGenerator.h"
#include "Exif.h"
#include <QDirIterator>
#include <QThreadPool>

/**
 * Scan monitored folders for photos
 * @return the photos
 */
QList<Photo*> Scanner::scan()
{
    QList<Photo*> photos;

    // Collect files
    QStringList monitoredFolders = Settings::getInstance()->getMonitoredFolders();
    foreach (const QString& folder, monitoredFolders)
    {
        QDirIterator it(folder, Settings::getInstance()->getMonitoredFileTypes().split(";"),
                        QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            it.next();
            if (Library::getInstance()->getPhoto(it.filePath()) != 0)   // already exists
                continue;

            // create a photo object
            Photo* photo = Photo::fromFileInfo(it.fileInfo());
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
