#include "Geocoder.h"
#include "Photo.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QtMath>
#define _USE_MATH_DEFINES
#include <math.h>

Geocoder::Geocoder(QObject* parent)
    : QObject(parent)
{
    _networkAccessManager = new QNetworkAccessManager(this);
    connect(_networkAccessManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(onReady(QNetworkReply*)));
}

void Geocoder::start(const QList<Photo*>& photos)
{
    _photos.clear();
    for (Photo* photo: photos)
        if (canDecode(photo))
            _photos << photo;
    processNext();
}

void Geocoder::processNext()
{
    if (_photos.isEmpty())
    {
        emit finished();
        return;
    }

    Photo* photo = _photos.front();
    QString latitude    = photo->getExif().getValue("GPS Latitude");
    QString longitude   = photo->getExif().getValue("GPS Longitude");
    QString url = tr("https://maps.googleapis.com/maps/api/geocode/json?"
                     "latlng=%1,%2&location_type=ROOFTOP&key=AIzaSyC3RwR2lIxzy83W8coGG8ZXdwG_rumNR6Q")
            .arg(coordinate2Decimal(latitude)).arg(coordinate2Decimal(longitude));
    _networkAccessManager->get(QNetworkRequest(url));
}

void Geocoder::onReady(QNetworkReply* reply)
{
    _photos.dequeue()->setAddress(parse(reply->readAll()));
    processNext();
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

//{
//   "results" : [
//      {
//         "address_components" : [
//            {
//               "long_name" : "95110",
//               "short_name" : "95110",
//               "types" : [ "postal_code" ]
//            },
//            {
//               "long_name" : "Central San Jose",
//               "short_name" : "Central San Jose",
//               "types" : [ "neighborhood", "political" ]
//            },
//            {
//               "long_name" : "San Jose",
//               "short_name" : "San Jose",
//               "types" : [ "locality", "political" ]
//            },
//            {
//               "long_name" : "Santa Clara County",
//               "short_name" : "Santa Clara County",
//               "types" : [ "administrative_area_level_2", "political" ]
//            },
//            {
//               "long_name" : "California",
//               "short_name" : "CA",
//               "types" : [ "administrative_area_level_1", "political" ]
//            },
//            {
//               "long_name" : "United States",
//               "short_name" : "US",
//               "types" : [ "country", "political" ]
//            }
//         ],
//         "formatted_address" : "San Jose, CA 95110, USA",
//         "geometry" : {
//            "bounds" : {
//               "northeast" : {
//                  "lat" : 37.3270671,
//                  "lng" : -121.8913613
//               },
//               "southwest" : {
//                  "lat" : 37.326348,
//                  "lng" : -121.8922297
//               }
//            },
//            "location" : {
//               "lat" : 37.3266982,
//               "lng" : -121.8919253
//            },
//            "location_type" : "ROOFTOP",
//            "viewport" : {
//               "northeast" : {
//                  "lat" : 37.3280565302915,
//                  "lng" : -121.8904465197085
//               },
//               "southwest" : {
//                  "lat" : 37.32535856970851,
//                  "lng" : -121.8931444802915
//               }
//            }
//         },
//         "place_id" : "ChIJB7-Tyq_Mj4ARMHU6wF9U4wE",
//         "types" : [ "premise" ]
//      },
//      {
//         "address_components" : [
//            {
//               "long_name" : "180",
//               "short_name" : "180",
//               "types" : [ "street_number" ]
//            },
//            {
//               "long_name" : "Woz Way",
//               "short_name" : "Woz Way",
//               "types" : [ "route" ]
//            },
//            {
//               "long_name" : "Central San Jose",
//               "short_name" : "Central San Jose",
//               "types" : [ "neighborhood", "political" ]
//            },
//            {
//               "long_name" : "San Jose",
//               "short_name" : "San Jose",
//               "types" : [ "locality", "political" ]
//            },
//            {
//               "long_name" : "Santa Clara County",
//               "short_name" : "Santa Clara County",
//               "types" : [ "administrative_area_level_2", "political" ]
//            },
//            {
//               "long_name" : "California",
//               "short_name" : "CA",
//               "types" : [ "administrative_area_level_1", "political" ]
//            },
//            {
//               "long_name" : "United States",
//               "short_name" : "US",
//               "types" : [ "country", "political" ]
//            },
//            {
//               "long_name" : "95110",
//               "short_name" : "95110",
//               "types" : [ "postal_code" ]
//            }
//         ],
//         "formatted_address" : "180 Woz Way, San Jose, CA 95110, USA",
//         "geometry" : {
//            "location" : {
//               "lat" : 37.3268616,
//               "lng" : -121.8920935
//            },
//            "location_type" : "ROOFTOP",
//            "viewport" : {
//               "northeast" : {
//                  "lat" : 37.3282105802915,
//                  "lng" : -121.8907445197085
//               },
//               "southwest" : {
//                  "lat" : 37.3255126197085,
//                  "lng" : -121.8934424802915
//               }
//            }
//         },
//         "place_id" : "ChIJua2uSa7Mj4ARAupFFMGM93w",
//         "types" : [ "street_address" ]
//      }
//   ],
//   "status" : "OK"
//}

// Parse the returing json as shown above
Address Geocoder::parse(const QByteArray& json)
{
    Address result;
    QJsonObject jsonObj = QJsonDocument::fromJson(json).object();
    if (jsonObj.value("status").toString() != "OK")
        return result;

    result.streetName   = findComponent(jsonObj, "route");
    result.cityName     = findComponent(jsonObj, "locality");
    result.stateName    = findComponent(jsonObj, "administrative_area_level_1");
    result.countryName  = findComponent(jsonObj, "country");
    return result;
}

// Find the value of a given type of component
QString Geocoder::findComponent(const QJsonObject& resultsObj, const QString& typeName) const
{
    QJsonArray resultsArray = resultsObj.value("results").toArray();
    for (const QJsonValue& resultValue: resultsArray)
    {
        QJsonObject addressObj = resultValue.toObject();
        QJsonArray componentsArray = addressObj.value("address_components").toArray();
        for (const QJsonValue& componentValue: componentsArray)
        {
            QJsonObject componentObj = componentValue.toObject();
            QJsonArray typeArray = componentObj.value("types").toArray();
            if (typeArray.contains(typeName))
                return componentObj.value("long_name").toString();
        }
    }
    return QString();
}

bool Geocoder::canDecode(Photo* photo) const
{
    QString latitude    = photo->getExif().getValue("GPS Latitude");
    QString longitude   = photo->getExif().getValue("GPS Longitude");
    return !latitude.isEmpty() && !longitude.isEmpty();
}

///////////////////////////////////////////////////////////////////////////////////
QString Address::toString() const
{
    QString result;
    if (!streetName.isEmpty())
        result += streetName;
    if (!cityName.isEmpty())
        result += ", " + cityName;
    if (!stateName.isEmpty())
        result += ", " + stateName;
    if (!countryName.isEmpty())
        result += ", " + countryName;
    return result;
}

Coordinates::Coordinates(double latitude, double longitude)
    : _latitude (latitude),
      _longitude(longitude) {}

double Coordinates::distanceTo(const Coordinates& another) const
{
    const double earthRadius = 6378.1370;

    double latitude1  = qDegreesToRadians(_latitude);
    double longitude1 = qDegreesToRadians(_longitude);
    double latitude2  = qDegreesToRadians(another._latitude);
    double longitude2 = qDegreesToRadians(another._longitude);

    double dLatitude  = latitude2  - latitude1;
    double dLongitude = longitude2 - longitude1;

    double a = qPow(qSin(dLatitude / 2.0), 2) + qCos(latitude1) * qCos(latitude2)
                    * qPow(qSin(dLongitude / 2.0), 2);
    double c = 2.0 * qAtan2(qSqrt(a), qSqrt(1.0 - a));
    double d = earthRadius * c;

    return d;
}
