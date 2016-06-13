#include "EventPage.h"
#include "ItemModel.h"
#include "EventModel.h"

#include <QSortFilterProxyModel>

EventPage::EventPage(QWidget *parent) :
    FilterPage(parent)
{
    ui.setupUi(this);
    connect(ui.btShowAll, SIGNAL(clicked()), SLOT(onShowAll()));
    connect(ui.tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(onItemDoubleClicked(QModelIndex)));
}

void EventPage::setModel(ItemModel* model)
{
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui.tableView->setModel(proxyModel);
    FilterPage::setModel(model);
}

void EventPage::update() {
    _model->update();
}

void EventPage::onItemDoubleClicked(const QModelIndex& idx)
{
    QString eventName = _model->data(_model->index(idx.row(), EventModel::COL_NAME)).toString();
    emit filter(eventName);
}

void EventPage::onShowAll()
{
    ui.tableView->clearSelection();
    emit filter(QString());
}
