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

protected:
    void mouseDoubleClickEvent(QMouseEvent*);
    void focusInEvent(QFocusEvent*);
    void focusOutEvent(QFocusEvent*);

private slots:
    void onTitleEdited(const QString& title);

signals:
    void itemSelected(Photo* photo);

private:
    Photo*          _photo;
    QLabel*         _thumbnail;
    EditableLabel*  _title;
    QColor          _backgroundColor;
};



#endif // PHOTOITEM_H
