#include "MonitoredFoldersDlg.h"
#include "Settings.h"
#include "SettingsDlg.h"

#include <QToolTip>

SettingsDlg::SettingsDlg(QWidget *parent) :
    QDialog(parent)
{
    ui.setupUi(this);
    connect(ui.btMonitoredFolders, SIGNAL(clicked(bool)), this, SLOT(onMonitoredFolders()));

    _settings = Settings::getInstance();
    ui.leMonitoredFileTypes ->setText(_settings->getMonitoredFileTypes());
    ui.leExiftool           ->setText(_settings->getExiftoolPath());
    ui.leFfmpeg             ->setText(_settings->getFfmpegPath());
}

void SettingsDlg::accept()
{
    _settings->setMonitoredFileTypes(ui.leMonitoredFileTypes->text());
    _settings->setExiftoolPath      (ui.leExiftool          ->text());
    _settings->setFfmpegPath        (ui.leFfmpeg            ->text());

    QDialog::accept();
}

void SettingsDlg::onMonitoredFolders()
{
    MonitoredFoldersDlg dlg(this);
    dlg.exec();
}
