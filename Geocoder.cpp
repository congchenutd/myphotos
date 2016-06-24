#include "Geocoder.h"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QRegularExpression>

Geocoder::Geocoder(QObject* parent)
    : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
    connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReady(QNetworkReply*)));
}

void Geocoder::getLocation(const QString& latitude, const QString& longitude)
{
    QString url = tr("https://maps.googleapis.com/maps/api/geocode/json?latlng=%1,%2&key=AIzaSyC3RwR2lIxzy83W8coGG8ZXdwG_rumNR6Q")
            .arg(coordinate2Decimal(latitude)).arg(coordinate2Decimal(longitude));
    _networkAccessManager->get(QNetworkRequest(url));
}

void Geocoder::onReady(QNetworkReply* reply)
{

}

// Convert coordinate, e.g., 37 deg 19' 53.28" N to decimal, e.g., 37.331467
QString Geocoder::coordinate2Decimal(const QString& coordinate)
{
    QRegularExpression re("(\\d+)\\sdeg\\s(\\d+)\\'\\s(\\d+\\.?\\d+)\\\"\\s(\\w)");
    QRegularExpressionMatch match = re.match(coordinate);
    if (match.hasMatch())
    {
        QString degrees     = match.captured(1).simplified();
        QString minutes     = match.captured(2).simplified();
        QString seconds     = match.captured(3).simplified();
        QString orientation = match.captured(4).simplified();
        double decimal = degrees.toInt() + minutes.toDouble() / 60 + seconds.toDouble() / 3600;
        QString minus = (orientation == "W" || orientation == "S") ? "-" : QString();
        return tr("%1%2").arg(minus).arg(decimal);
    }
    return QString();
}
