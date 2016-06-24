#include "Photo.h"
#include "ThumbnailGenerator.h"
#include "Settings.h"
#include "ThumbnailDAO.H"

#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QUuid>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

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

Thumbnail* generateThumbnail(Photo* photo)
{
    QSize size = Settings::getInstance()->getNewThumbnailSize();
    QImage image;
    if (photo->isVideo())
    {
        using namespace cv;

        // WORKAROUND: rename the file, because opencv can't open file path containing Chinese
        QFileInfo fileInfo(photo->getFilePath());
        QString tempFilePath = fileInfo.path() + QDir::separator() +
                QUuid::createUuid().toString() + "." + fileInfo.suffix();
        QFile::rename(photo->getFilePath(), tempFilePath);

        VideoCapture cap(tempFilePath.toStdString());

        if (cap.isOpened())
        {
            cap.set(CV_CAP_PROP_POS_FRAMES, 1); // capture the 1st frame
            Mat frame;
            cap >> frame;
            image = Mat2QImage(frame).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        }
        // Restore the file name
        QFile::rename(tempFilePath, photo->getFilePath());
    }
    else
    {
        QImageReader reader(photo->getFilePath());
        reader.setAutoTransform(true);
        image = reader.read().scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // thumbnail file name is the path of the photo
    QString fileName = photo->getFilePath() + ".png";
    fileName.replace(QDir::separator(), '-');

    // generate thumbnail file path
    QString location = Settings::getInstance()->getThumbnailCacheLocation();
    QString filePath = location + QDir::separator() + fileName;

    // save thumbnail file
    image.save(filePath);

    return new Thumbnail(ThumbnailDAO::getInstance()->getNextID(), filePath);
}
