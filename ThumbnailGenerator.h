#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>

class Photo;
class Thumbnail;

Thumbnail* generateThumbnail(Photo* photo);

#endif // THUMBNAILGENERATOR_H
