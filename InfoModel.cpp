#include "InfoModel.h"
#include "Photo.h"
#include "Exif.h"

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

    QMap<QString, QString> data = photo->getExif().getData();
    for (QMap<QString, QString>::const_iterator it = data.begin(); it != data.end(); ++it)
    {
        int lastRow = rowCount();
        insertRow(lastRow);
        setData(index(lastRow, 0), it.key());
        setData(index(lastRow, 1), it.value());
    }
    sort(COL_PROPERTY);

    // top 3 rows are reserved for basic photo info
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

    // Only basic photo info is saved
    _photo->setTitle    (data(index(ROW_TITLE,  COL_VALUE)).toString());
    _photo->setFilePath (data(index(ROW_PATH,   COL_VALUE)).toString());
    _photo->setTimeTaken(data(index(ROW_TIME,   COL_VALUE)).toDateTime());

    Exif exif = _photo->getExif();
    for (int row = ROW_TIME + 1; row < rowCount(); ++row)
        exif.setValue(data(index(row, COL_PROPERTY)).toString(), data(index(row, COL_VALUE)).toString());
}

