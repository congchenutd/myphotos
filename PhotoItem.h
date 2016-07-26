#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include <QThread>
#include <QWidget>

class Photo;
class QLabel;
class EditableLabel;
class ClusterView;
class Event;

/**
 * A widget in the PhotoView representing a Photo
 */
class PhotoItem : public QWidget
{
    Q_OBJECT

public:
    PhotoItem(Photo* photo, ClusterView* clusterView);
    void setPhoto(Photo* photo);
    void setSelected(bool selected);
    Photo* getPhoto();
    void rename();
    void resizeThumbnail();
    QRect geometryMappedTo(const QWidget* widget) const;
    ClusterView* getClusterView() const;
    void updateTitleVisibility();

public slots:
    void setTitle(const QString& title);

protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void paintEvent(QPaintEvent* event);

private:
    ClusterView*    _clusterView;
    Photo*          _photo;
    EditableLabel*  _title;
    QLabel*         _thumbnail;
    QColor          _backgroundColor;
    QSize           _thumbnailSize;
    QLabel*         _videoLabel;
    QLabel*         _favoriteLabel;
};

#endif // PHOTOITEM_H
