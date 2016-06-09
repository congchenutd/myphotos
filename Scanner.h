#ifndef SCANNER_H
#define SCANNER_H

#include <QFileInfo>
#include <QFutureWatcher>
#include <QImage>
#include <QObject>
#include <QQueue>
#include <QSize>
#include <QList>

class Photo;
class Library;

class Scanner: public QObject
{
    Q_OBJECT

public:
    Scanner();
    int scan();

private slots:
    void onThumbnailCreated(int index);

signals:
    void photoAdded(Photo* photo);

private:
    QList<Photo*>           _photos;
    QFutureWatcher<QImage>* _scalingWatcher;
    Library*                _library;
};

#endif // SCANNER_H
