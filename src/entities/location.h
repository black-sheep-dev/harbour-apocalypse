#ifndef LOCATION_H
#define LOCATION_H

#include <QObject>

class Location : public QObject
{
    Q_OBJECT

    Q_PROPERTY(float latitude READ latitude WRITE setLatitude NOTIFY latitudeChanged)
    Q_PROPERTY(float longitude READ longitude WRITE setLongitude NOTIFY longitudeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Location(QObject *parent = nullptr);

    float latitude() const;
    float longitude() const;
    QString name() const;

signals:
    void latitudeChanged(float latitude);
    void longitudeChanged(float longitude);
    void nameChanged(const QString &name);

public slots:
    void setLatitude(float latitude);
    void setLongitude(float longitude);
    void setName(const QString &name);

private:
    float m_latitude{0.0};
    float m_longitude{0.0};
    QString m_name;
};

#endif // LOCATION_H
