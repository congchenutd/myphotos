#include "Photo.h"
#include "PhotoItem.h"
#include "Thumbnail.h"
#include "EditableLabel.h"
#include "Library.h"

#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>

PhotoItem::PhotoItem(Photo* photo)
    : _photo(photo)
{
    _thumbnail = new QLabel(this);
    _thumbnail->setPixmap(QPixmap(photo->getThumbnail()->getFilePath()));
    _thumbnail->setAlignment(Qt::AlignCenter);

    _title = new EditableLabel;
    _title->setText(photo->getTitle());
    _title->setAlignment(Qt::AlignCenter);
    connect(_title, SIGNAL(editingFinished(QString)), this, SLOT(onTitleEdited(QString)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_thumbnail);
    layout->addWidget(_title);
}

void PhotoItem::mouseDoubleClickEvent(QMouseEvent*) {
    _title->edit();
}

void PhotoItem::onTitleEdited(const QString& title)
{
    _photo->setTitle(title);
    Library::getInstance()->save();
}
