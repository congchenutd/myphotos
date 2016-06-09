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

QImage scale(Photo* photo)
{
    QImageReader reader(photo->getFilePath());
    reader.setAutoTransform(true);
    return reader.read().scaled(Settings::getInstance()->getThumbnailSize(),
                                Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

Scanner::Scanner()
{
    _library = Library::getInstance();
    _scalingWatcher = new QFutureWatcher<QImage>(this);
    connect(_scalingWatcher, SIGNAL(resultReadyAt(int)), SLOT(onThumbnailCreated(int)));
}

int Scanner::scan()
{
    _photos.clear();
    QStringList folders = Settings::getInstance()->getMonitoredFolders();
    for (const QString& folder: folders)
    {
        QDir dir(folder);
        dir.setFilter(QDir::Files | QDir::NoSymLinks);
        dir.setSorting(QDir::Time | QDir::Reversed);
        dir.setNameFilters(Settings::getInstance()->getMonitoredFileTypes().split(";"));
        for (const QFileInfo& info: dir.entryInfoList())
            if (Library::getInstance()->getPhoto(info.filePath()) == 0) // doesn't exist
            {
                Photo* photo = new Photo(PhotoDAO::getInstance()->getNextID(),
                                         info.baseName(),
                                         info.filePath(),
                                         info.lastModified());
                photo->save();
                _photos << photo;
            }
    }

    _scalingWatcher->setFuture(QtConcurrent::mapped(_photos, scale));
    return _photos.length();
}

void Scanner::onThumbnailCreated(int index)
{
    Photo* photo = _photos.at(index);

    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString fileName = photo->getFilePath();
    fileName.replace(QDir::separator(), '-');
    QString filePath = location + QDir::separator() + fileName;

    QImage image = _scalingWatcher->resultAt(index);
    image.save(filePath);

    Thumbnail* thumbnail = new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), filePath);
    photo->setThumbnail(thumbnail);

    _library->addPhoto(photo);
    _library->addThumbnail(thumbnail);
    thumbnail->save();
    photo->save();

    emit photoAdded(photo);
}
