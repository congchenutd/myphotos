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
