#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QObject>

#include <QGeoPositionInfoSource>
#include <QHash>
#include <QJsonObject>
#include <QRectF>

#include "src/entities/message.h"
#include "src/models/locationmodel.h"

class MessageHelper : public QObject
{
    Q_OBJECT

public:
    explicit MessageHelper(LocationModel *model, QObject *parent = nullptr);

    void enableLog(bool enable = true);
    bool parseMessage(const QJsonObject &obj, Message *msg);

private:
    QGeoPositionInfoSource *m_source{QGeoPositionInfoSource::createDefaultSource(this)};
    LocationModel *m_locationModel{nullptr};
    bool m_log{false};
};

#endif // MESSAGEHELPER_H
