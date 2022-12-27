#ifndef LOCATION_H
#define LOCATION_H

#include <QJsonObject>
#include <QString>

struct Location
{
    float lat{0.0};
    float lon{0.0};
    QString name;

    QJsonObject toJson() const {
        QJsonObject obj;
        obj.insert("name", this->name);
        obj.insert("lat", this->lat);
        obj.insert("lon", this->lon);
        return obj;
    }
};

#endif // LOCATION_H
