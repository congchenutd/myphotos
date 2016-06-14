#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoDAO.h"
#include "ThumbnailDAO.h"
#include <QDir>
#include <QImageReader>
#include <QtConcurrent>

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

Scanner::Scanner()
{
    _library = Library::getInstance();
    _scalingWatcher = new QFutureWatcher<QImage>(this);
    connect(_scalingWatcher, SIGNAL(resultReadyAt(int)), SLOT(onThumbnailCreated(int)));
}

/**
 * Scan monitored folders for photos
 * @return the number of photos
 */
int Scanner::scan()
{
    _photos.clear();

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
                _photos << photo;
            }
    }

    // Map photos to thumbnails
    _scalingWatcher->setFuture(QtConcurrent::mapped(_photos, scale));
    return _photos.length();
}

void Scanner::onThumbnailCreated(int index)
{
    Photo* photo = _photos.at(index);

    // thumbnail file name is the path of the photo
    QString fileName = photo->getFilePath();
    fileName.replace(QDir::separator(), '-');

    // generate thumbnail file path
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString filePath = location + QDir::separator() + fileName;

    // save thumbnail file
    QImage image = _scalingWatcher->resultAt(index);
    image.save(filePath);

    // create thumbnail object
    Thumbnail* thumbnail = new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), filePath);
    photo->setThumbnail(thumbnail);

    _library->addPhoto(photo);
    _library->addThumbnail(thumbnail);
    thumbnail->save();
    photo->save();

    emit photoAdded(photo);
}
