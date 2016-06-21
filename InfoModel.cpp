#include "InfoModel.h"
#include "Photo.h"

#include <QProcess>

InfoModel::InfoModel(QObject* parent)
    : QStandardItemModel(parent),
      _photo(0)
{}

void InfoModel::setPhoto(Photo* photo)
{
    removeRows(0, rowCount());
    _photo = photo;
    if (photo == 0)
        return;

    QProcess* process = new QProcess(this);
    process->start("/usr/local/bin/exiftool", QStringList() << photo->getFilePath());
    process->waitForFinished();

    QStringList list = QString(process->readAllStandardOutput()).split("\n");
    foreach (QString line, list)
    {
        QStringList sections = line.split(":");
        if (sections.size() == 2)
        {
            int lastRow = rowCount();
            insertRow(lastRow);
            setData(index(lastRow, 0), sections.at(0).simplified());
            setData(index(lastRow, 1), sections.at(1).simplified());
        }
    }
    sort(0);

    if (photo != 0)
    {
        insertRows(0, 3);
        setData(index(ROW_TITLE,    COL_PROPERTY),  tr("Title"));
        setData(index(ROW_TITLE,    COL_VALUE),     photo->getTitle());
        setData(index(ROW_PATH,     COL_PROPERTY),  tr("File path"));
        setData(index(ROW_PATH,     COL_VALUE),     photo->getFilePath());
        setData(index(ROW_TIME,     COL_PROPERTY),  tr("Time taken"));
        setData(index(ROW_TIME,     COL_VALUE),     photo->getTimeTaken().toString("yyyy-MM-dd HH:mm:ss"));
    }
}

void InfoModel::save()
{
    if (_photo == 0)
        return;

    _photo->setTitle    (data(index(ROW_TITLE,  COL_VALUE)).toString());
    _photo->setFilePath (data(index(ROW_PATH,   COL_VALUE)).toString());
    _photo->setTimeTaken(data(index(ROW_TIME,   COL_VALUE)).toDateTime());
}

