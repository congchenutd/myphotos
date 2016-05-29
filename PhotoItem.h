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

protected:
    void mouseDoubleClickEvent(QMouseEvent*);

private slots:
    void onTitleEdited(const QString& title);

private:
    Photo*          _photo;
    QLabel*         _thumbnail;
    EditableLabel*  _title;
};

#endif // PHOTOITEM_H
