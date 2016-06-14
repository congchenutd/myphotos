#include "MonitoredFoldersDlg.h"
#include "Settings.h"
#include <QFileDialog>
#include <QMessageBox>

MonitoredFoldersDlg::MonitoredFoldersDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    _model.setStringList(Settings::getInstance()->getMonitoredFolders());
    ui.listView->setModel(&_model);

    connect(ui.btAdd,       SIGNAL(clicked(bool)), SLOT(onAdd()));
    connect(ui.btRemove,    SIGNAL(clicked(bool)), SLOT(onRemove()));
}

void MonitoredFoldersDlg::accept()
{
    Settings::getInstance()->setMonitoredFolders(_model.stringList());
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
        _model.setData(_model.index(lastRow), dir);
    }
}

void MonitoredFoldersDlg::onRemove()
{
    QModelIndexList rows = ui.listView->selectionModel()->selectedRows();
    if (rows.isEmpty())
        return;

    if (QMessageBox::warning(this, tr("Warning"),
                             tr("Are you sure to remove this folder?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        _model.removeRow(rows.front().row());
}
