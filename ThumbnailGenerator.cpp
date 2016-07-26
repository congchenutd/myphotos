#include "Photo.h"
#include "ThumbnailGenerator.h"
#include "Settings.h"
#include "ThumbnailDAO.H"

#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QProcess>
#include <QDebug>

Thumbnail* generateThumbnail(const Photo* photo)
{
    QSize size = Settings::getInstance()->getNewThumbnailSize();
    QString thumbnailFilePath = createThumbnailFilePath(photo);

    if (photo->isVideo())
    {
        QString ffmpegPath = Settings::getInstance()->getFfmpegPath();
        if (!ffmpegPath.isEmpty())
        {
            QString command = QObject::tr("%1 -y -i \"%2\" -vframes 1 -filter:v scale=\'500:-1\' \"%3\"")
                    .arg(ffmpegPath)
                    .arg(photo->getFilePath())
                    .arg(thumbnailFilePath);
            QProcess::execute(command);
        }
    }
    else
    {
        QImageReader reader(photo->getFilePath());
        reader.setAutoTransform(true);
        reader.read().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(thumbnailFilePath);
    }

    return new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), thumbnailFilePath);
}

QString createThumbnailFilePath(const Photo *photo)
{
    // thumbnail file name is the path of the photo
    QString fileName = photo->getFilePath() + ".png";
    fileName.replace(QDir::separator(), '-');

    // generate thumbnail file path
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    return location + QDir::separator() + fileName;
}
