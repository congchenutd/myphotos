#include "MonitoredFoldersDlg.h"
#include "Settings.h"
#include "SettingsDlg.h"

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btMonitoredFolders, SIGNAL(clicked(bool)), this, SLOT(onMonitoredFolders()));

    Settings* settings = Settings::getInstance();
    ui.leMonitoredFileTypes->setText(settings->getMonitoredFileTypes());
}

void SettingsDlg::accept()
{
    Settings::getInstance()->setMonitoredFileTypes(ui.leMonitoredFileTypes->text());
    QDialog::accept();
}

void SettingsDlg::onMonitoredFolders()
{
    MonitoredFoldersDlg dlg(this);
    dlg.exec();
}
