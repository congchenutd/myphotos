#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoDAO.h"
#include "ThumbnailDAO.h"
#include "ThumbnailGenerator.h"
#include "PhotoInfo.h"
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

Scanner::Scanner()
{
    _library = Library::getInstance();
    _thumbnailFutureWatcher = new QFutureWatcher<Thumbnail*>(this);
    _infoFutureWatcher      = new QFutureWatcher<PhotoInfo*>(this);
    connect(_thumbnailFutureWatcher,    SIGNAL(resultReadyAt(int)), SLOT(onThumbnailCreated(int)));
    connect(_infoFutureWatcher,         SIGNAL(resultReadyAt(int)), SLOT(onPhotoInfoExtracted(int)));
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

    qDebug() << QDateTime::currentDateTime();

    _thumbnailFutureWatcher->setFuture(QtConcurrent::mapped(_photos, generateThumbnail));
    _infoFutureWatcher->setFuture(QtConcurrent::mapped(_photos, extractPhotoInfo));
    return _photos.length();
}

void Scanner::onThumbnailCreated(int index)
{
    Photo* photo = _photos.at(index);
    Thumbnail* thumbnail = _thumbnailFutureWatcher->resultAt(index);
    photo->setThumbnail(thumbnail);

    _library->addPhoto(photo);
    _library->addThumbnail(thumbnail);
    thumbnail->save();
    photo->save();

    emit photoAdded(photo);
}

void Scanner::onPhotoInfoExtracted(int index)
{
    Photo* photo = _photos.at(index);
    PhotoInfo* info = _infoFutureWatcher->resultAt(index);
    photo->setInfo(info);
    photo->save();
}
