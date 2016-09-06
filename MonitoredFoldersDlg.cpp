#include "MonitoredFoldersDlg.h"
#include "Settings.h"
#include "ImageColorBoolProxy.h"
#include "ImageColorBoolDelegate.h"
#include <QFileDialog>
#include <QMessageBox>

MonitoredFoldersDlg::MonitoredFoldersDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    ui.btRemove->setEnabled(false);

    load();

    ImageColorBoolProxy* proxy = new ImageColorBoolProxy(this);
    proxy->setColumnType(COL_CHECKED, ImageColorBoolProxy::BoolColumn);
    proxy->setSourceModel(&_model);

    ui.tableView->setModel(proxy);
    ImageColorBoolDelegate* delegate = new ImageColorBoolDelegate(proxy, ui.tableView);
    delegate->setEditTrigger(QEvent::MouseButtonPress);
    delegate->setCheckedImage  (QPixmap(":/Images/Checked.png"));
    delegate->setUncheckedImage(QPixmap(":/Images/UnChecked.png"));
    ui.tableView->setItemDelegate(delegate);

    ui.tableView->resizeColumnToContents(COL_FOLDER);

    connect(ui.btAdd,       SIGNAL(clicked(bool)), SLOT(onAdd()));
    connect(ui.btRemove,    SIGNAL(clicked(bool)), SLOT(onRemove()));
    connect(ui.tableView->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(onCurrentRowChanged(QModelIndex)));
}

void MonitoredFoldersDlg::onCurrentRowChanged(const QModelIndex& idx) {
    ui.btRemove->setEnabled(idx.isValid());
}

void MonitoredFoldersDlg::accept()
{
    save();
    QDialog::accept();
}

void MonitoredFoldersDlg::onAdd()
{
    QString dir = QFileDialog::getExistingDirectory(
                this, tr("Add Monitored Folder"), ".",
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty())
    {
        int lastRow = _model.rowCount();
        _model.insertRow(lastRow);
        _model.setData(_model.index(lastRow, COL_FOLDER ), dir);
        _model.setData(_model.index(lastRow, COL_CHECKED), true);
        ui.tableView->resizeColumnToContents(COL_FOLDER);
    }
}

void MonitoredFoldersDlg::onRemove()
{
    QModelIndexList rows = ui.tableView->selectionModel()->selectedIndexes();
    if (!rows.isEmpty())
        _model.removeRow(rows.front().row());
}

void MonitoredFoldersDlg::load()
{
    _model.setColumnCount(2);
    _model.setHeaderData(COL_FOLDER,  Qt::Horizontal, tr("Folder"));
    _model.setHeaderData(COL_CHECKED, Qt::Horizontal, tr("Enabled"));

    QStringList folders = Settings::getInstance()->getMonitoredFolders();
    _model.setRowCount(folders.size());
    for(int row = 0; row < folders.size(); ++ row)
    {
        QStringList sections = folders.at(row).split(_separator);
        if(sections.size() == 2)
        {
            _model.setData(_model.index(row, COL_FOLDER ), sections[COL_FOLDER]);
            _model.setData(_model.index(row, COL_CHECKED), sections[COL_CHECKED]);
        }
    }
}

void MonitoredFoldersDlg::save()
{
    _model.sort(COL_FOLDER);
    QStringList fields;
    for(int row = 0; row < _model.rowCount(); ++ row)
    {
        QString folder   = _model.data(_model.index(row, COL_FOLDER )).toString();
        QString selected = _model.data(_model.index(row, COL_CHECKED)).toString();
        if(!folder.isEmpty() && !selected.isEmpty())
            fields << (QStringList() << folder << selected).join(_separator);
    }
    Settings::getInstance()->setMonitoredFolders(fields);
}
