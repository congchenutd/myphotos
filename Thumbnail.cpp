#include "Thumbnail.h"
#include "Settings.h"
#include "Photo.h"
#include "ThumbnailDAO.h"
#include <QDir>

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

Thumbnail* Thumbnail::fromPhoto(Photo* photo)
{
    QString fileName = createThumbnailFileName(photo->getFilePath());
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString filePath = location + QDir::separator() + fileName;

//    ThumbnailThread* thread = new ThumbnailThread(photo, filePath);
//    thread->start();

    QPixmap pixmap = QPixmap(photo->getFilePath());
    QSize size = Settings::getInstance()->getThumbnailSize();
    pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(filePath);

    return new Thumbnail(getNextID(), filePath);
}

QString Thumbnail::createThumbnailFileName(const QString& filePath)
{
    QString result = filePath;
    result.replace(QDir::separator(), '-');
    return result;
}

ThumbnailThread::ThumbnailThread(Photo* photo, const QString& filePath)
    : _photo(photo),
      _filePath(filePath)
{}

void ThumbnailThread::run()
{
    QSize size = Settings::getInstance()->getThumbnailSize();
    QPixmap pixmap = QPixmap(_photo->getFilePath());
    pixmap.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation).save(_filePath);
}
