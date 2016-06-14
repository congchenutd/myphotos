#ifndef MONITOREDFOLDERSDLG_H
#define MONITOREDFOLDERSDLG_H

#include "ui_MonitoredFoldersDlg.h"

#include <QStringListModel>

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

private:
    Ui::MonitoredFoldersDlg ui;

    QStringListModel _model;
};

#endif // MONITOREDFOLDERSDLG_H
