#ifndef MESSAGEHELPER_H
#define MESSAGEHELPER_H

#include <QObject>

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
    QHash<quint16, QRectF> geocodeRects() const;
    bool parseMessage(const QJsonObject &obj, Message *msg);
    void setGeocodeRects(const QJsonObject &obj);
    void setGeocodeRects(const QHash<quint16, QRectF> & rects);

private:
    QHash<quint16, QRectF> m_geocodeRects;
    LocationModel *m_locationModel{nullptr};
    bool m_log{false};
};

#endif // MESSAGEHELPER_H
