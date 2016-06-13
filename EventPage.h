#ifndef EVENTPAGE_H
#define EVENTPAGE_H

#include "FilterPage.h"
#include "ui_EventPage.h"

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

#endif // EVENTPAGE_H
