#include "Photo.h"
#include "PhotoItem.h"
#include "Thumbnail.h"
#include "EditableLabel.h"
#include "Library.h"

#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPalette>
#include <QUrl>
#include <QDesktopServices>

PhotoItem::PhotoItem(Photo* photo)
    : _selected(false),
      _videoLabel(0),
      _thumbnailSize(100)
{
    _thumbnail  = new QLabel(this);
    _thumbnail->setAlignment(Qt::AlignCenter);

    _title = new EditableLabel;
    _title->setAlignment(Qt::AlignCenter);
    connect(_title, SIGNAL(editingFinished(QString)), this, SLOT(onTitleEdited(QString)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_thumbnail);
    layout->addWidget(_title);

    _backgroundColor = palette().background().color();
    setAutoFillBackground(true);

    _videoLabel = new QLabel(this);
    setPhoto(photo);
}

void PhotoItem::setPhoto(Photo* photo)
{
    if (photo == 0)
        return;

    _photo = photo;
    _title->setText(photo->getTitle());
    resizeThumbnail(_thumbnailSize);

    if (photo->exists() && photo->isVideo())
    {
        _videoLabel->setPixmap(QPixmap(":/Images/Video.png"));
        _videoLabel->show();
        _videoLabel->move(16, 10);
    }
    else {
        _videoLabel->hide();
    }
}

void PhotoItem::mouseDoubleClickEvent(QMouseEvent*)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_photo->getFilePath()));
}

void PhotoItem::onTitleEdited(const QString& title)
{
    _photo->setTitle(title);
    _photo->save();
    emit titleChanged(title);
}

/**
 * Update selection status
 */
void PhotoItem::setSelected(bool selected)
{
    _selected = selected;
    QPalette palette(this->palette());
    palette.setColor(QPalette::Background,
                     selected ? palette.highlight().color() : _backgroundColor);
    setPalette(palette);
}

Photo* PhotoItem::getPhoto() {
    return _photo;
}

void PhotoItem::rename() {
    _title->edit();
}

void PhotoItem::resizeThumbnail(int size)
{
    _thumbnailSize = size;
    QPixmap pixmap = (getPhoto()->exists() && getPhoto()->getThumbnail() != 0) ?
                QPixmap(_photo->getThumbnail()->getFilePath()) :
                QPixmap(":/Images/Error.png");
    _thumbnail->setPixmap(pixmap.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
