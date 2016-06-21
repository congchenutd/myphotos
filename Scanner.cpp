#include "Scanner.h"
#include "Settings.h"
#include "Library.h"
#include "Photo.h"
#include "Thumbnail.h"
#include "PhotoDAO.h"
#include "ThumbnailDAO.h"
#include "ThumbnailGenerator.h"
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

                QSize size = Settings::getInstance()->getNewThumbnailSize();
                if (ThumbnailGenerator* generator = ThumbnailGenerator::getGenerator(photo, size))
                {
                    generator->setAutoDelete(false);
                    connect(generator, SIGNAL(finished(QImage)), SLOT(onThumbnailCreated(QImage)));
                    QThreadPool::globalInstance()->setMaxThreadCount(5);
                    QThreadPool::globalInstance()->start(generator);
                }
            }
    }

    return _photos.length();
}

void Scanner::onThumbnailCreated(const QImage& image)
{
    Photo* photo = static_cast<ThumbnailGenerator*>(sender())->getPhoto();

    // thumbnail file name is the path of the photo
    QString fileName = photo->getFilePath() + ".png";
    fileName.replace(QDir::separator(), '-');

    // generate thumbnail file path
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString filePath = location + QDir::separator() + fileName;

    // save thumbnail file
    image.save(filePath);

    // create thumbnail object
    Thumbnail* thumbnail = new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), filePath);
    photo->setThumbnail(thumbnail);

    _library->addPhoto(photo);
    _library->addThumbnail(thumbnail);
    thumbnail->save();
    photo->save();

    emit photoAdded(photo);
    delete sender();
}
