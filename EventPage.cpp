#include "EventPage.h"
#include "ItemModel.h"
#include "EventModel.h"

#include <QDateEdit>
#include <QSortFilterProxyModel>

EventPage::EventPage(QWidget *parent) :
    FilterPage(parent)
{
    ui.setupUi(this);

    ui.tableView->setItemDelegateForColumn(EventModel::COL_DATE, new DateItemDelegate);

    connect(ui.btShowAll, SIGNAL(clicked()), SLOT(onShowAll()));
    connect(ui.tableView, SIGNAL(doubleClicked(QModelIndex)), SLOT(onItemDoubleClicked(QModelIndex)));
}

void EventPage::setModel(ItemModel* model)
{
    // for sorting
    QSortFilterProxyModel* proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(model);
    ui.tableView->setModel(proxyModel);
    FilterPage::setModel(model);

    ui.tableView->resizeColumnsToContents();
    ui.tableView->sortByColumn(EventModel::COL_DATE, Qt::DescendingOrder);
}

void EventPage::update() {
    getModel()->update();
}

void EventPage::onItemDoubleClicked(const QModelIndex& idx)
{
    // NOTE: use proxy model, not the source model, because the data may be sorted
    QAbstractItemModel* model = ui.tableView->model();
    QString eventName = model->data(model->index(idx.row(), EventModel::COL_NAME)).toString();
    emit filter(eventName);
}

void EventPage::onShowAll()
{
    ui.tableView->clearSelection();
    emit filter(QString());
}

////////////////////////////////////////////////////////////////////////////////////////////
QString DateItemDelegate::displayText(const QVariant& value, const QLocale& locale) const {
    return value.toDate().toString("yyyy-MM-dd");
}

QWidget* DateItemDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                                        const QModelIndex& index) const
{
    QWidget* widget = QStyledItemDelegate::createEditor(parent, option, index);
    if (QDateEdit* editor = qobject_cast<QDateEdit*>(widget))
    {
        editor->setDisplayFormat("yyyy-MM-dd");
        return editor;
    }
    return widget;
}
