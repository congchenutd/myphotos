#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include "ui_PhotoView.h"
#include "Clustering.h"
#include "PhotoClusters.h"
#include <QList>

class Library;
class Photo;
class PhotoItem;
class QLayoutItem;
class NewItemMenu;
class SortableVBoxLayout;
class ClusterView;

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
    void resizeThumbnails();
    void addPhoto(Photo* photo);
    PhotoItem* getItem(Photo* photo) const;

public slots:
    void sort();

private slots:
    void onTagChecked   (bool checked);
    void onPeopleChecked(bool checked);
    void onEventChecked (bool checked);
    void onLocationDecoded(Photo* photo);

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
    Library*            _library;
    QPoint              _clickedPosition;
    QRubberBand*        _rubberBand;
    QSet<PhotoItem*>    _selected;
    QString             _sortBy;
    bool                _ascending;
    SortableVBoxLayout* _vBoxLayout;
    PhotoClusters       _photoClusters;
    QMap<Cluster*, ClusterView*>    _cluster2ClusterView;
};

// Comparators for sorting
class ClusterViewLessAddress
{
public:
    ClusterViewLessAddress(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class ClusterViewLessDate
{
public:
    ClusterViewLessDate(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class PhotoItemLessTime
{
public:
    PhotoItemLessTime(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

class PhotoItemLessTitle
{
public:
    PhotoItemLessTitle(bool lessThan = true);
    bool operator() (QLayoutItem* lhs, QLayoutItem* rhs) const;
private:
    bool _lessThan;
};

#endif // PHOTOVIEW_H
