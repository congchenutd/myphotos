#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class Photo;
class QProgressBar;
class Scanner;
class QSlider;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    static MainWindow* getInstance();
    QAction* getRenameAction();
    QAction* getRemoveAction();
    QAction* getDeleteAction();
    QAction* getSortByTitleAction();
    QAction* getSortByTimeAction();
    QAction* getSortingOrderAction();

protected:
    void closeEvent(QCloseEvent*);

private slots:
    void onScan();
    void onOptions();
    void onPhotoAdded(Photo* photo);
    void onSortingOrder();
    void sort();
    void onPhotoSelected(const QList<PhotoItem*>& selected);
    void onRename();
    void onRemove();
    void onDelete();
    void onThumbnailSize(int size);
    void onNewTag   (const QString& tagValue);
    void onNewPeople(const QString& name);
    void onNewEvent (const QString& name, const QDate& date);
    void onFilterByTags     (const QStringList& tags,   bool AND);
    void onFilterByPeople   (const QStringList& people, bool AND);
    void onFilterByEvent    (const QString& eventName);
    void onAbout();
    void onInfoChanged(Photo* photo);

private:
    void resetPhotos();

private:
    Ui::MainWindow ui;
    QProgressBar*   _progressBar;
    QSlider*        _slider;
    bool            _ascending;
    Library*        _library;
    Scanner*        _scanner;

    static MainWindow*  _instance;
};

#endif // MAINWINDOW_H
