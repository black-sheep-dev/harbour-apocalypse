#ifndef SERVICE_H
#define SERVICE_H

#include <QJsonObject>
#include <QString>

struct Service
{
    bool active{false};
    QString id;
    QString name;
    QString url;

    QJsonObject toJson() const {
        QJsonObject obj;

        obj.insert("active", this->active);
        obj.insert("id", this->id);
        obj.insert("name", this->name);
        obj.insert("url", this->url);

        return obj;
    }
};

#endif // SERVICE_H
