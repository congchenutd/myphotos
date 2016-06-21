#include "InfoModel.h"
#include "InfoPage.h"
#include "Photo.h"

#include <QProcess>
#include <QStandardItemModel>

InfoPage::InfoPage(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    _model = new InfoModel(this);
    _model->setColumnCount(2);
    _model->setHeaderData(0, Qt::Horizontal, tr("Property"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Value"));

    ui.tableView->setModel(_model);
    connect(ui.tableView->itemDelegate(), SIGNAL(closeEditor(QWidget*, QAbstractItemDelegate::EndEditHint)),
            SLOT(onEditFinished()));
}

void InfoPage::setCurrentPhoto(Photo* photo) {
    _model->setPhoto(photo);
}

void InfoPage::onEditFinished() {
    _model->save();
}
