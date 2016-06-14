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
    ui.leMonitoredFileTypes->setText(_settings->getMonitoredFileTypes());
    ui.sliderThumbnailSize->setValue(_settings->getNewThumbnailSize().width());
}

void SettingsDlg::accept()
{
    _settings->setMonitoredFileTypes(ui.leMonitoredFileTypes->text());

    int width = ui.sliderThumbnailSize->value();
    _settings->setNewThumbnailSize(QSize(width, width));

    QDialog::accept();
}

void SettingsDlg::onMonitoredFolders()
{
    MonitoredFoldersDlg dlg(this);
    dlg.exec();
}
