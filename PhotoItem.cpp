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
    : _photo(photo),
      _selected(false)
{
    _thumbnail = new QLabel(this);
    if (photo->getThumbnail() != 0)
        _thumbnail->setPixmap(QPixmap(photo->getThumbnail()->getFilePath()));
    _thumbnail->setAlignment(Qt::AlignCenter);

    _title = new EditableLabel;
    _title->setText(photo->getTitle());
    _title->setAlignment(Qt::AlignCenter);
    connect(_title, SIGNAL(editingFinished(QString)), this, SLOT(onTitleEdited(QString)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_thumbnail);
    layout->addWidget(_title);

    setFocusPolicy(Qt::StrongFocus);
    _backgroundColor = palette().background().color();
}

void PhotoItem::mouseDoubleClickEvent(QMouseEvent*)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(_photo->getFilePath()));
}

void PhotoItem::onTitleEdited(const QString& title)
{
    _photo->setTitle(title);
    _photo->save();
    emit titleEdited(title);
}

void PhotoItem::setSelected(bool selected)
{
    _selected = selected;
    if (selected)
    {
        QPalette palette(this->palette());
        palette.setColor(QPalette::Background, palette.highlight().color());
        setAutoFillBackground(true);
        setPalette(palette);
    }
    else
    {
        QPalette palette(this->palette());
        palette.setColor(QPalette::Background, _backgroundColor);
        setAutoFillBackground(true);
        setPalette(palette);
    }
}

Photo* PhotoItem::getPhoto() {
    return _photo;
}

void PhotoItem::rename() {
    _title->edit();
}

void PhotoItem::resizeThumbnail(int size)
{
    QPixmap pixmap = QPixmap(_photo->getThumbnail()->getFilePath());
    _thumbnail->setPixmap(pixmap.scaled(size, size, Qt::KeepAspectRatio));
}


