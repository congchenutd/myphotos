#ifndef GEOCODER_H
#define GEOCODER_H

#include <QObject>

class QNetworkAccessManager;
class QNetworkReply;

class Geocoder : public QObject
{
    Q_OBJECT
public:
    explicit Geocoder(QObject* parent = 0);
    void queryLocation(const QString& latitude, const QString& longitude);

private slots:
    void onReady(QNetworkReply* reply);

private:
    static QString coordinate2Decimal(const QString& coordinates);

signals:
    void locationAvailable(const QString& location);

private:
    QNetworkAccessManager* _networkAccessManager;
};

struct Location
{
    QString streetNumber;
    QString streetName;
    QString cityName;
    QString propertyName;
};

#endif // GEOCODER_H
