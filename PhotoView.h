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

/**
 * A view widget for photos
 */
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
    void addPhoto(Photo* photo);

public slots:
    void sort();

private slots:
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
    int                 getClickedItemIndex (const QPoint& point) const;
    PhotoItem*          getClickedItem      (const QPoint& point) const;
    QSet<PhotoItem *>   getClickedItems     (const QPoint& start, const QPoint& end) const;
    void toggleSelection(PhotoItem* clicked);
    NewItemMenu* createTagMenu();
    NewItemMenu* createPeopleMenu();
    NewItemMenu* createEventMenu();

    QList<PhotoItem*> getAllPhotoItems() const;
    PhotoItem* getItemAt(int index) const;

private:
    Ui::PhotoView ui;
    FlowLayout*         _layout;
    Library*            _library;
    QPoint              _clickedPosition;
    QRubberBand*        _rubberBand;
    QSet<PhotoItem*>    _selected;
    QString             _sortBy;
    bool                _ascending;
    int                 _thumbnailSize;
};

// Comparators for sorting
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
