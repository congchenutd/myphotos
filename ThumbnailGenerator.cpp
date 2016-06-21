#include "Photo.h"
#include "ThumbnailGenerator.h"

#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QUuid>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

ThumbnailGenerator::ThumbnailGenerator(Photo* photo, const QSize& size)
    : _photo(photo),
      _size(size) {}

Photo *ThumbnailGenerator::getPhoto() const {
    return _photo;
}

ThumbnailGenerator* ThumbnailGenerator::getGenerator(Photo* photo, const QSize& size) {
    if (photo->isVideo())
        return new VideoThumbnailGenerator(photo, size);
    else
        return new ImageThumbnailGenerator(photo, size);
}

/////////////////////////////////////////////////////////////////////////////////////////
ImageThumbnailGenerator::ImageThumbnailGenerator(Photo* photo, const QSize& size)
    : ThumbnailGenerator(photo, size) {}

void ImageThumbnailGenerator::run()
{
    QImageReader reader(_photo->getFilePath());
    reader.setAutoTransform(true);
    emit finished(reader.read().scaled(_size,
                                Qt::KeepAspectRatio, Qt::SmoothTransformation));
}


////////////////////////////////////////////////////////////////////////////////////////
QImage Mat2QImage(cv::Mat const& mat)
{
     cv::Mat temp;
     cvtColor(mat, temp, CV_BGR2RGB);
     QImage image((const uchar*) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     image.bits(); // enforce deep copy
     return image;
}

cv::Mat QImage2Mat(QImage const& image)
{
     cv::Mat temp(image.height(), image.width(), CV_8UC3, (uchar*) image.bits(), image.bytesPerLine());
     cv::Mat mat;
     cvtColor(temp, mat, CV_BGR2RGB);
     return mat;
}

VideoThumbnailGenerator::VideoThumbnailGenerator(Photo* photo, const QSize& size)
    : ThumbnailGenerator(photo, size) {}

void VideoThumbnailGenerator::run()
{
    using namespace cv;

    // WORKAROUND: rename the file, because opencv can't open file path containing Chinese
    QFileInfo fileInfo(_photo->getFilePath());
    QString tempFilePath = fileInfo.path() + QDir::separator() +
            QUuid::createUuid().toString() + "." + fileInfo.suffix();
    QFile::rename(_photo->getFilePath(), tempFilePath);

    VideoCapture cap(tempFilePath.toStdString());

    if (cap.isOpened())
    {
        cap.set(CV_CAP_PROP_POS_FRAMES, 1);
        Mat frame;
        cap >> frame;
        emit finished(Mat2QImage(frame).scaled(_size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
        emit finished(QImage());

    // Restore the file name
    QFile::rename(tempFilePath, _photo->getFilePath());
}
