#ifndef TAGSVIEW_H
#define TAGSVIEW_H

#include "FilterPage.h"

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QRadioButton;
class ItemModel;

class TagPage : public FilterPage
{
    Q_OBJECT

public:
    explicit TagPage(QWidget* parent = 0);
    void update();

private slots:
    void onTagChecked();
    void onShowAll();

signals:
    void filterByTags(const QStringList& tags, bool AND);

private:
    QVBoxLayout*    _layoutTags;
    QHBoxLayout*    _layoutAnd;
    QVBoxLayout*    _layoutTop;
    QPushButton*    _btShowAll;
    QRadioButton*   _radioAnd;
    QRadioButton*   _radioOr;
};

#endif // TAGSVIEW_H
