#ifndef MONITOREDFOLDERSDLG_H
#define MONITOREDFOLDERSDLG_H

#include "ui_MonitoredFoldersDlg.h"

#include <QStandardItemModel>

/**
 * A dialog for editing monitored folders
 */
class MonitoredFoldersDlg : public QDialog
{
    Q_OBJECT

public:
    explicit MonitoredFoldersDlg(QWidget* parent = 0);
    void accept();

private slots:
    void onAdd();
    void onRemove();
    void onCurrentRowChanged(const QModelIndex& idx);

private:
    void load();
    void save();

private:
    Ui::MonitoredFoldersDlg ui;
    enum {COL_FOLDER, COL_CHECKED};

    QStandardItemModel _model;
    const static char  _separator = ';';
};

#endif // MONITOREDFOLDERSDLG_H
