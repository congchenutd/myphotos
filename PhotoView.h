#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include "ui_PhotoView.h"
#include <functional>
#include <QList>

class Library;
class Photo;
class PhotoItem;
class QLayoutItem;
class FlowLayout;
class NewItemMenu;

class PhotoView : public QWidget
{
    Q_OBJECT

public:
    explicit PhotoView(QWidget* parent = 0);

    void clear();
    void load(const QList<Photo*>& photos);
    void sort(const QString& byWhat, bool ascending);
    QList<PhotoItem*> getSelectedItems() const;
    void removeItem(PhotoItem* item);
    void resizeThumbnails(int size);

public slots:
    void addPhoto(Photo* photo);
    void sort();

private slots:
    void onNewTag(const QString& tagValue);
    void onTagChecked   (bool checked);
    void onPeopleChecked(bool checked);
    void onEventChecked (bool checked);

signals:
    void photoSelected(Photo*);
    void selectionChanged(const QList<PhotoItem*>&);
    void newTag     (const QString& name);
    void newPeople  (const QString& name);
    void newEvent   (const QString& name, const QDate& date);

protected:
    void mousePressEvent    (QMouseEvent* event);
    void mouseMoveEvent     (QMouseEvent* event);
    void mouseReleaseEvent  (QMouseEvent* event);

private:
    void updateSelection();
    PhotoItem* getClickedItem(const QPoint& point);
    NewItemMenu* createTagMenu();
    NewItemMenu* createPeopleMenu();
    NewItemMenu* createEventMenu();

private slots:
    void onItemSelected(bool selected);

private:
    Ui::PhotoView ui;
    FlowLayout*         _layout;
    Library*            _library;
    QPoint              _selectionStart;
    QRubberBand*        _rubberBand;
    QList<PhotoItem*>   _selected;
    QString             _sortBy;
    bool                _ascending;
};

// Comparators
struct PhotoItemLessTitle {
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
};

struct PhotoItemGreaterTitle {
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
};

struct PhotoItemLessTime {
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
};

struct PhotoItemGreaterTime {
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
};


#endif // PHOTOVIEW_H
