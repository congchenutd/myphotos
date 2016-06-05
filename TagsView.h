#ifndef TAGSVIEW_H
#define TAGSVIEW_H

#include <QWidget>

class QVBoxLayout;
class QHBoxLayout;
class QPushButton;
class QRadioButton;

class TagsView : public QWidget
{
    Q_OBJECT

public:
    explicit TagsView(QWidget* parent = 0);
    void setTags(const QStringList& tags);

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
