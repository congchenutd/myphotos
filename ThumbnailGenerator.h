#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>

class Photo;
class Thumbnail;

Thumbnail*  generateThumbnail       (const Photo* photo);
QString     createThumbnailFilePath (const Photo* photo);

#endif // THUMBNAILGENERATOR_H
