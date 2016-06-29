#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include <QWidget>

class Photo;
class QLabel;
class EditableLabel;

/**
 * A widget in the PhotoView representing a Photo
 */
class PhotoItem : public QWidget
{
    Q_OBJECT

public:
    PhotoItem(Photo* photo);
    void setPhoto(Photo* photo);
    void setSelected(bool selected);
    Photo* getPhoto();
    void rename();
    void resizeThumbnail();

protected:
    void mouseDoubleClickEvent(QMouseEvent*);

private slots:
    void onTitleEdited(const QString& title);

signals:
    void titleChanged(const QString&);

private:
    Photo*          _photo;
    QLabel*         _thumbnail;
    EditableLabel*  _title;
    QColor          _backgroundColor;
    bool            _selected;
    QLabel*         _videoLabel;
};



#endif // PHOTOITEM_H
