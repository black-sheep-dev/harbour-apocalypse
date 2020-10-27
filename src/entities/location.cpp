#include "location.h"

Location::Location(QObject *parent) :
    QObject(parent)
{

}

float Location::latitude() const
{
    return m_latitude;
}

float Location::longitude() const
{
    return m_longitude;
}

QString Location::name() const
{
    return m_name;
}

void Location::setLatitude(float latitude)
{
    if (qFuzzyCompare(m_latitude, latitude))
        return;

    m_latitude = latitude;
    emit latitudeChanged(m_latitude);
}

void Location::setLongitude(float longitude)
{
    if (qFuzzyCompare(m_longitude, longitude))
        return;

    m_longitude = longitude;
    emit longitudeChanged(m_longitude);
}

void Location::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}
