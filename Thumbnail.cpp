#include "Thumbnail.h"
#include "Settings.h"
#include "Photo.h"
#include "ThumbnailDAO.h"
#include <QDir>
#include <QImageReader>

Thumbnail::Thumbnail(int id, const QString& filePath)
    : Persistable(id, ThumbnailDAO::getInstance()),
      _filePath(filePath)
{}

QString Thumbnail::getFilePath() const {
    return _filePath;
}

void Thumbnail::setFilePath(const QString& path) {
    _filePath = path;
}


ThumbnailThread::ThumbnailThread(Photo* photo)
    : _photo(photo)
{
    connect(this, SIGNAL(finished()), this, SLOT(onFinished()));
}

QString ThumbnailThread::createThumbnailFileName(const QString& filePath)
{
    QString result = filePath;
    result.replace(QDir::separator(), '-');
    return result;
}

void ThumbnailThread::run()
{
    QString location    = Settings::getInstance()->getThumbnailCacheLocation();
    QSize   size        = Settings::getInstance()->getThumbnailSize();
    QString fileName = createThumbnailFileName(_photo->getFilePath());
    _filePath = location + QDir::separator() + fileName;

    QImageReader reader(_photo->getFilePath());
    reader.setAutoTransform(true);
    reader.read().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(_filePath);
}

void ThumbnailThread::onFinished()
{
    Thumbnail* thumbnail = new Thumbnail(Thumbnail::getNextID(), _filePath);
    _photo->setThumbnail(thumbnail);
    emit finished(_photo);
}
