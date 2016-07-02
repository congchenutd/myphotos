#include "Photo.h"
#include "ThumbnailGenerator.h"
#include "Settings.h"
#include "ThumbnailDAO.H"

#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QProcess>
#include <QDebug>

Thumbnail* generateThumbnail(Photo* photo)
{
    QSize size = Settings::getInstance()->getNewThumbnailSize();

    // thumbnail file name is the path of the photo
    QString fileName = photo->getFilePath() + ".png";
    fileName.replace(QDir::separator(), '-');

    // generate thumbnail file path
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString filePath = location + QDir::separator() + fileName;

    if (photo->isVideo())
    {
        QString command = QObject::tr("/usr/local/Cellar/ffmpeg/2.8.4/bin/ffmpeg -y \
                                      -i \"%1\" -vframes 1 -filter:v scale=\'500:-1\' \"%2\"")
                .arg(photo->getFilePath()).arg(filePath);
        QProcess::execute(command);
    }
    else
    {
        QImageReader reader(photo->getFilePath());
        reader.setAutoTransform(true);
        reader.read().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(filePath);
    }

    return new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), filePath);
}
