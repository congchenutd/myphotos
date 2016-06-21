#include "FrameExtractor.h"
#include "Photo.h"
#include "ThumbnailGenerator.h"

#include <QImageReader>
#include <QLabel>
#include <QMediaPlayer>
#include <QProcess>
#include <QVideoProbe>
#include <QVideoWidget>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

ThumbnailGenerator::ThumbnailGenerator(Photo* photo, const QSize& size)
    : _photo(photo),
      _size(size) {}

Photo *ThumbnailGenerator::getPhoto() const {
    return _photo;
}

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

    VideoCapture cap((const char*) _photo->getFilePath().toUtf8());
    if (!cap.isOpened())
        return;

    cap.set(CV_CAP_PROP_POS_FRAMES, 10);
    Mat frame;
    cap >> frame;
    emit finished(Mat2QImage(frame).scaled(_size,
                               Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
