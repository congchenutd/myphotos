#include "MonitoredFoldersDlg.h"
#include "SettingsDlg.h"

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btMonitoredFolders, SIGNAL(clicked(bool)), this, SLOT(onMonitoredFolders()));
}

void SettingsDlg::onMonitoredFolders()
{
    MonitoredFoldersDlg dlg(this);
    dlg.exec();
}
