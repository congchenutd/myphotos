#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include "ui_PhotoView.h"
#include <functional>
#include <QList>

class Library;
class Photo;
class PhotoItem;
struct PhotoItemComparator;

class PhotoView : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoView(QWidget* parent = 0);

    void load();
    void sort(const QString& byWhat, bool ascending);

public slots:
    void addPhoto(Photo* photo);

signals:
    void photoSelected(Photo*);

protected:
    void mousePressEvent    (QMouseEvent* event);
    void mouseMoveEvent     (QMouseEvent* event);
    void mouseReleaseEvent  (QMouseEvent*);

private:
    PhotoItemComparator* getComparator(const QString& byWhat, bool ascending);
    void selectItems();

private:
    Ui::PhotoView ui;
    QLayout*            _layout;
    Library*            _library;
    QPoint              _selectionStart;
    QRubberBand*        _rubberBand;
    QList<PhotoItem*>   _selected;
};

struct PhotoItemComparator: public std::less<PhotoItem*> {
    virtual bool operator() (PhotoItem* lhs, PhotoItem* rhs) const = 0;
};

struct PhotoItemLessTitle: public PhotoItemComparator {
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;
};

struct PhotoItemGreaterTitle: public PhotoItemComparator {
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;
};

struct PhotoItemLessTime: public PhotoItemComparator {
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;
};

struct PhotoItemGreaterTime: public PhotoItemComparator {
    bool operator() (PhotoItem* lhs, PhotoItem* rhs) const;
};

#endif // PHOTOVIEW_H
