#ifndef GEOCODER_H
#define GEOCODER_H

#include <QObject>
#include <QQueue>

class QNetworkAccessManager;
class QNetworkReply;
class Photo;

struct Address
{
    QString streetName;
    QString cityName;
    QString stateName;
    QString countryName;

    QString toString() const;
};

struct Coordinates
{
    double _latitude;
    double _longitude;

    Coordinates(double latitude, double longitude);
    double distanceTo(const Coordinates& another) const;
};

class Geocoder : public QObject
{
    Q_OBJECT
public:
    explicit Geocoder(QObject* parent = 0);
    void start(const QList<Photo*>& photos);

private slots:
    void onReady(QNetworkReply* reply);

private:
    void processNext();
    static QString coordinate2Decimal(const QString& coordinates);
    Address parse(const QByteArray& json);
    QString findComponent(const QJsonObject& resultsObj, const QString& typeName) const;
    bool canDecode(Photo* photo) const;

signals:
    void decoded(Photo* photo);

private:
    QNetworkAccessManager*  _networkAccessManager;
    QQueue<Photo*>          _photos;
};

#endif // GEOCODER_H
