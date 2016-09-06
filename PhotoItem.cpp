#include "Photo.h"
#include "PhotoItem.h"
#include "Thumbnail.h"
#include "EditableLabel.h"
#include "Library.h"
#include "Settings.h"
#include "ClusterView.h"

#include <QLabel>
#include <QPixmap>
#include <QVBoxLayout>
#include <QPalette>
#include <QUrl>
#include <QDesktopServices>

PhotoItem::PhotoItem(Photo* photo, ClusterView* clusterView)
    : _clusterView(clusterView),
      _videoLabel(0)
{
    _thumbnail  = new QLabel(this);
    _thumbnail->setAlignment(Qt::AlignCenter);

    _title = new EditableLabel;
    _title->setAlignment(Qt::AlignCenter);
    updateTitleVisibility();
    connect(_title, SIGNAL(editingFinished(QString)), this, SLOT(setTitle(QString)));

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(_thumbnail);
    layout->addWidget(_title);

    _backgroundColor = palette().background().color();
    setAutoFillBackground(true);

    _videoLabel     = new QLabel(this);
    _favoriteLabel  = new QLabel(this);
    setPhoto(photo);
}

void PhotoItem::setPhoto(Photo* photo)
{
    if (photo == 0)
        return;

    _photo = photo;
    _title->setText(photo->getTitle());

    // show video icon if it's a video
    _videoLabel->setPixmap(QPixmap(":/Images/Video.png").scaled(24, 24));
    _videoLabel->move(16, 10);
    _videoLabel->setVisible(photo->exists() && photo->isVideo());

    _favoriteLabel->setPixmap(QPixmap(":/Images/Heart2.png"));
    _favoriteLabel->move(50, 10);
    _favoriteLabel->setVisible(photo->isFavorite());
}

void PhotoItem::mouseDoubleClickEvent(QMouseEvent*) {
    QDesktopServices::openUrl(QUrl::fromLocalFile(_photo->getFilePath()));
}

void PhotoItem::paintEvent(QPaintEvent* event)
{
    resizeThumbnail();
    updateTitleVisibility();
    QWidget::paintEvent(event);
}

void PhotoItem::setTitle(const QString& title)
{
    if (_title->text() == title)
        return;

    _title->setText(_photo->setTitle(title));
    _photo->save();
}

/**
 * Update selection status
 */
void PhotoItem::setSelected(bool selected)
{
    // highlight
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

void PhotoItem::resizeThumbnail()
{
    // resize only when it's invalid and visible
    QSize size = Settings::getInstance()->getThumbnailSize();
    if (_thumbnailSize == size || visibleRegion().isEmpty())
        return;

    _thumbnailSize = size;
    QPixmap pixmap = (getPhoto()->exists() && getPhoto()->getThumbnail() != 0) ?
                    QPixmap(_photo->getThumbnail()->getFilePath()) :
                    QPixmap(":/Images/Error.png");
    _thumbnail->setPixmap(pixmap.scaled(_thumbnailSize, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

/**
 * @return geometry using a given widget's coordinates
 */
QRect PhotoItem::geometryMappedTo(const QWidget* widget) const
{
    QPoint topLeft = mapTo(widget, mapFromParent(geometry().topLeft()));
    return QRect(topLeft, size());
}

ClusterView* PhotoItem::getClusterView() const {
    return _clusterView;
}

void PhotoItem::updateTitleVisibility()
{
    bool show = Settings::getInstance()->getShowTitle();
    if (!visibleRegion().isEmpty())
        _title->setVisible(show);
}
