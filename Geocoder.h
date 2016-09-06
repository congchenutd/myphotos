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

/**
 * Geocoder decode a GPS coordinate into an Address
 */
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
    bool canDecode(Photo* photo) const;
    QString parse(const QByteArray& json, Address& address);
    QString findComponent(const QJsonObject& resultsObj, const QString& typeName) const;

    QString getKey() const;


    static QString coordinate2Decimal(const QString& coordinates);

signals:
    void decoded(Photo* photo);

private:
    QNetworkAccessManager*  _networkAccessManager;
    QQueue<Photo*>          _photos;
    QStringList             _apiKeys;
    int                     _keyIndex;
};

#endif // GEOCODER_H
