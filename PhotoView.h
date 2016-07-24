#ifndef PHOTOVIEW_H
#define PHOTOVIEW_H

#include "PhotoClusters.h"
#include <QList>
#include <QScrollArea>
#include <QSet>
#include <QWidget>

class Library;
class Photo;
class PhotoItem;
class QLayoutItem;
class NewItemMenu;
class SortableVBoxLayout;
class ClusterView;
class QRubberBand;

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
    void removePhotoItem(PhotoItem* item);
    void addPhoto(Photo* photo);
    PhotoItem* getItem(Photo* photo) const;
    void updateTitles();

public slots:
    void resizeThumbnails();
    void sort();

private slots:
    void onTagChecked   (bool checked);
    void onPeopleChecked(bool checked);
    void onEventChecked (bool checked);
    void onLocationDecoded(Photo* photo);
    void onSelectAll();
    void onSetFavorite(bool favorite);

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
    void updateSelection(const QList<PhotoItem*>& selected);
    int                 getClickedItemIndex (const QPoint& point) const;
    PhotoItem*          getClickedItem      (const QPoint& point) const;
    QList<PhotoItem*> getClickedItems(const QPoint& start, const QPoint& end) const;
    void toggleSelection(PhotoItem* clicked);
    NewItemMenu* createTagMenu();
    NewItemMenu* createPeopleMenu();
    NewItemMenu* createEventMenu();
    QAction*     createFavoriteAction();

    QList<PhotoItem*> getAllPhotoItems() const;
    PhotoItem* getItemAt(int index) const;

private:
    Library*                        _library;
    QPoint                          _clickedPosition;
    QRubberBand*                    _rubberBand;
    QList<PhotoItem*>               _selected;
    QString                         _sortBy;
    bool                            _ascending;
    SortableVBoxLayout*             _vBoxLayout;
    PhotoClusters                   _photoClusters;     // the model
    QMap<Cluster*, ClusterView*>    _clusterViews;
};


#endif // PHOTOVIEW_H
