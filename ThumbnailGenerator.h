#ifndef THUMBNAILGENERATOR_H
#define THUMBNAILGENERATOR_H

#include <QObject>
#include <QRunnable>


class Photo;
class Thumbnail;

Thumbnail* generateThumbnail(Photo* photo);

#endif // THUMBNAILGENERATOR_H
