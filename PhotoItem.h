#ifndef PHOTOITEM_H
#define PHOTOITEM_H

#include <QWidget>

class Photo;
class QLabel;
class EditableLabel;

class PhotoItem : public QWidget
{
    Q_OBJECT

public:
    PhotoItem(Photo* photo);
    void setSelected(bool selected);
    Photo* getPhoto();
    void rename();
    void resizeThumbnail(int size);

protected:
    void mouseDoubleClickEvent(QMouseEvent*);

private slots:
    void onTitleEdited(const QString& title);

signals:
    void selectionChanged(bool selected);
    void titleEdited(const QString&);

private:
    Photo*          _photo;
    QLabel*         _thumbnail;
    EditableLabel*  _title;
    QColor          _backgroundColor;
    bool            _selected;
};



#endif // PHOTOITEM_H
