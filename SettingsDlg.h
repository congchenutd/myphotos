#ifndef SETTINGSDLG_H
#define SETTINGSDLG_H

#include "ui_SettingsDlg.h"

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDlg(QWidget* parent = 0);
    void accept();

private slots:
    void onMonitoredFolders();

private:
    Ui::SettingsDlg ui;
};

#endif // SETTINGSDLG_H
