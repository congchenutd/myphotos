#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoDAO.h"
#include "ThumbnailDAO.h"
#include "ThumbnailGenerator.h"
#include "Exif.h"
#include <QDir>
#include <QImageReader>
#include <QtConcurrent>
#include <QThreadPool>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QLabel>

/**
 * The Map function
 */
QImage scale(Photo* photo)
{
    QImageReader reader(photo->getFilePath());
    reader.setAutoTransform(true);
    return reader.read().scaled(Settings::getInstance()->getNewThumbnailSize(),
                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

/**
 * Scan monitored folders for photos
 * @return the number of photos
 */
int Scanner::scan()
{
    QList<Photo*> photos;
    int count = 0;

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
                photo->save();
                photos << photo;
                if (photos.length() == 10)
                {
                    ScannerThread* thread = new ScannerThread(QList<Photo*>(photos));
                    connect(thread, SIGNAL(photoAdded(Photo*)), SIGNAL(photoAdded(Photo*)));
                    QThreadPool::globalInstance()->start(thread);
                    photos.clear();
                }
                count ++;
            }
    }

    return count;
}

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

        photo->setExif(Exif(photo));
        photo->save();
        emit photoAdded(photo);
    }
}
