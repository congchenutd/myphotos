#include "InfoPage.h"
#include "Photo.h"

#include <QProcess>
#include <QStandardItemModel>

InfoPage::InfoPage(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    _model = new QStandardItemModel(this);
    _model->setColumnCount(2);
    _model->setHeaderData(0, Qt::Horizontal, tr("Property"));
    _model->setHeaderData(1, Qt::Horizontal, tr("Value"));

    ui.tableView->setModel(_model);

    _process = new QProcess(this);
    connect(_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onReadyRead()));
    connect(_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished()));
}

void InfoPage::setCurrentPhoto(Photo* photo)
{
    _model->removeRows(0, _model->rowCount());
    _output.clear();
    if (photo != 0)
        _process->start("/usr/local/bin/exiftool", QStringList() << photo->getFilePath());
}

/**
 * Receive output
 */
void InfoPage::onReadyRead() {
    _output.append(_process->readAllStandardOutput());
}

/**
 * Parse output and store exif into the model
 */
void InfoPage::onFinished()
{
    QStringList list = _output.split("\n");
    foreach (QString line, list)
    {
        QStringList sections = line.split(":");
        if (sections.size() == 2)
        {
            int lastRow = _model->rowCount();
            _model->insertRow(lastRow);
            _model->setData(_model->index(lastRow, 0), sections.at(0).simplified());
            _model->setData(_model->index(lastRow, 1), sections.at(1).simplified());
        }
    }
    ui.tableView->sortByColumn(0, Qt::AscendingOrder);
}
