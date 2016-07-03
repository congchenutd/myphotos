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
    Address parse(const QByteArray& json);
    QString findComponent(const QJsonObject& resultsObj, const QString& typeName) const;

    static QString coordinate2Decimal(const QString& coordinates);

signals:
    void decoded(Photo* photo);

private:
    QNetworkAccessManager*  _networkAccessManager;
    QQueue<Photo*>          _photos;
};

#endif // GEOCODER_H
