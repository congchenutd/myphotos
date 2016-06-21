#ifndef FRAMEEXTRACTOR_H
#define FRAMEEXTRACTOR_H

#include <QAbstractVideoSurface>

class FrameExtractor : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    FrameExtractor(QObject* parent = 0);


    QList<QVideoFrame::PixelFormat>
    supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const;

    bool present(const QVideoFrame& frame);

signals:
    void frameReady(const QImage& image);
};

#endif // FRAMEEXTRACTOR_H
