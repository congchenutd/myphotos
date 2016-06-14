#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include "FilterPage.h"
#include "ui_EventPage.h"

#include <QStyledItemDelegate>

class ItemModel;

class EventPage : public FilterPage
{
    Q_OBJECT

public:
    explicit EventPage(QWidget* parent = 0);
    void setModel(ItemModel* model);
    void update();

private slots:
    void onItemDoubleClicked(const QModelIndex& idx);
    void onShowAll();

signals:
    void filter(const QString& eventName);

private:
    Ui::EventPage ui;
};

/**
 * An item delegate to support customized date format
 */
class DateItemDelegate: public QStyledItemDelegate
{
public:
    QString displayText(const QVariant& value, const QLocale& locale) const;
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex&index) const;
};

#endif // EVENTPAGE_H
