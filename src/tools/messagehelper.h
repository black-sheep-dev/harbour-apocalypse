#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QObject>

#include <QJsonObject>

#include "src/entities/message.h"
#include "src/models/locationmodel.h"

class MessageHelper : public QObject
{
    Q_OBJECT
public:
    explicit MessageHelper(LocationModel *model, QObject *parent = nullptr);

    bool parseMessage(const QJsonObject &obj, Message *msg);

private:
    LocationModel *m_locationModel{nullptr};
};

#endif // MESSAGEHELPER_H
