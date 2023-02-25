#include "serviceprovider.h"

#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QPolygonF>
#include <QRectF>
#include <QSettings>
#include <QStandardPaths>

#include <nemonotifications-qt5/notification.h>

static const QByteArray MAPBOX_API_KEY = "AAAAYXicK8jWS630MvTMys+MzK3IiDQqqUp0D8tJdg/KScn0LPbMdc30z/TKSjSqyPHNTTdJ9IisSMktqPLNtSyOzDUx9Q/PMUky9q3yzDPQq8ooynJMSylNTTNN9soP94w0MDE1cQQAr/Ih5g==";

ServiceProvider::ServiceProvider(QObject *parent) :
    QDBusAbstractAdaptor(parent)
{
    readServices();
    readSettings();

    restartApiInterface();

    // check if network is available
    connect(m_nm, &NetworkManager::connectedChanged, [this]() {
        qDebug() << "Connected: " << m_nm->connected();
        this->m_offline = !m_nm->connected();
        emit this->offlineChanged();

        if (m_nm->connected()) this->refresh();
    });

    if (!m_autoUpdate) {
        refresh();
    } else {
        m_backgroundJob->run();
        m_backgroundJob->wait();
    }
}

ServiceProvider::~ServiceProvider()
{
    if (m_backgroundJob != nullptr) {
        m_backgroundJob->stop();
        m_backgroundJob->deleteLater();
    }
    writeSettings();
}

bool ServiceProvider::autoUpdate() const
{
    return m_autoUpdate;
}

void ServiceProvider::setAutoUpdate(bool enable)
{
    if (m_autoUpdate == enable)
        return;
    m_autoUpdate = enable;
    emit autoUpdateChanged();

    if (m_autoUpdate) {
        if (m_backgroundJob == nullptr) {
            m_backgroundJob = new BackgroundActivity(this);
            connect(m_backgroundJob, &BackgroundActivity::running, this, &ServiceProvider::refresh);

            m_backgroundJob->setWakeupFrequency(BackgroundActivity::Frequency(m_updateInterval));
        }
    } else {
        m_backgroundJob->deleteLater();
        m_backgroundJob = nullptr;
    }
}

bool ServiceProvider::gps() const
{
    return m_gps;
}

void ServiceProvider::setGps(bool enable)
{
    if (m_gps == enable)
        return;
    m_gps = enable;
    emit gpsChanged();

    if (m_gps) {
        if (m_gpsSource == nullptr) {
            m_gpsSource = QGeoPositionInfoSource::createDefaultSource(this);
            m_gpsSource->setUpdateInterval(30000);
            m_gpsSource->startUpdates();
        }
    } else {
        m_gpsSource->deleteLater();
        m_gpsSource = nullptr;
    }
}

bool ServiceProvider::loading() const
{
    return m_loading;
}

const QString &ServiceProvider::localMessages() const
{
    return m_localMessages;
}

int ServiceProvider::localSeverity() const
{
    return m_localSeverity;
}

QString ServiceProvider::locations() const
{
    QJsonArray arr;

    for (const auto &location : m_locations) {
        arr.append(location.toJson());
    }

    return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}

QString ServiceProvider::mapBoxApiKey() const
{
    return qUncompress(QByteArray().fromBase64(MAPBOX_API_KEY));
}

const QString &ServiceProvider::messages() const
{
    return m_messages;
}

bool ServiceProvider::notify() const
{
    return m_notify;
}

void ServiceProvider::setNotify(bool enabled)
{
    if (m_notify == enabled)
        return;
    m_notify = enabled;
    emit notifyChanged(m_notify);
}

bool ServiceProvider::offline() const
{
    return m_offline;
}

bool ServiceProvider::playSound() const
{
    return m_playSound;
}

void ServiceProvider::setPlaySound(bool enabled)
{
    if (m_playSound == enabled)
        return;
    m_playSound = enabled;
    emit playSoundChanged(m_playSound);
}

QString ServiceProvider::services() const
{
    QJsonArray arr;

    for (const auto &service : m_services) {
        arr.append(service.toJson());
    }

    return QJsonDocument(arr).toJson(QJsonDocument::Compact);
}

int ServiceProvider::updateInterval() const
{
    return m_updateInterval;
}

void ServiceProvider::setUpdateInterval(int interval)
{
    if (m_updateInterval == interval)
        return;
    m_updateInterval = interval;
    emit updateIntervalChanged();

    if (m_backgroundJob == nullptr) {
        return;
    }

    m_backgroundJob->setWakeupFrequency(BackgroundActivity::Frequency(m_updateInterval));
}

bool ServiceProvider::addLocation(const QString &name, double latitude, double longitude)
{
    for (const auto &location : m_locations) {
        if (location.name == name) return false;
    }

    Location loc;
    loc.name = name;
    loc.lat = latitude;
    loc.lon = longitude;

    m_locations.append(loc);
    emit locationsChanged();

    writeSettings();

    refresh();

    return true;
}

bool ServiceProvider::removeLocation(int index)
{
    if (index < 0 || index >= m_locations.length()) {
        return false;
    }

    m_locations.takeAt(index);

    writeSettings();

    refresh();

    return true;
}

bool ServiceProvider::updateLocation(int index, const QString &name, double latitude, double longitude)
{
    if (index < 0 || index >= m_locations.length()) {
        return false;
    }

    m_locations[index].name = name;
    m_locations[index].lat = latitude;
    m_locations[index].lon = longitude;
    emit locationsChanged();

    writeSettings();

    refresh();

    return true;
}

bool ServiceProvider::enableService(const QString &id, bool enable)
{
    for (auto &service : m_services) {
        if (service.id != id) continue;

        service.active = enable;
        emit servicesChanged();
        return true;
    }

    return false;
}

void ServiceProvider::refresh()
{
    if (!m_nm->connected()) {
        return;
    }

    // set loading
    m_loading = true;
    emit loadingChanged();

    // clear
    m_requestQueue.clear();
    m_localSeverity = 0;
    m_localMessageBuffer = QJsonArray();
    m_messageBuffer = QJsonArray();

    // send requests
    for (const auto &service : m_services) {
        if (!service.active) continue;


        m_requestQueue.append(service.url);
        m_api->fetch(service.url);
    }
}

void ServiceProvider::restartApiInterface()
{
    m_loading = false;
    m_requestQueue.clear();

    if (m_api != nullptr) {
        delete m_api;
    }

    m_api = new ApiInterface(this);
    connect(m_api, &ApiInterface::messagesAvailable, this, &ServiceProvider::parseMessages);
    connect(m_api, &ApiInterface::requestFailed, this, &ServiceProvider::onRequestFailed);
}

void ServiceProvider::saveSettings()
{
    writeSettings();
}

void ServiceProvider::shutdown()
{
    writeSettings();
}

void ServiceProvider::test()
{
    sendNotification(QJsonObject());
}

void ServiceProvider::onRequestFailed(const QString &url)
{
    m_requestQueue.removeAll(url);

    if (m_requestQueue.isEmpty()) {
        m_loading = false;
        emit loadingChanged();
    }
}

void ServiceProvider::parseMessages(const QString &url, const QJsonArray &msgs)
{
    if (m_backgroundJob != nullptr) {
        m_backgroundJob->wait();
    }

    int ret = m_requestQueue.removeAll(url);

    if (m_requestQueue.isEmpty()) {
        m_loading = false;
        emit loadingChanged();
    }

    if (ret <= 0) return;

    // parse messages
    for (const auto &v : msgs) {
        const auto obj = v.toObject();

        // create basic message
        QJsonObject msg = obj.value("info").toArray().first().toObject();
        msg.insert("identifier", obj.value("identifier"));
        msg.insert("sender", obj.value("sender"));
        msg.insert("sent", obj.value("sent"));
        msg.insert("status", obj.value("status"));
        msg.insert("msgType", obj.value("msgType"));
        msg.insert("scope", obj.value("scope"));

        // severity
        const auto value = msg.value("severity").toString();
        int severity{0};
        if (value == QLatin1String("Minor")) {
            severity = 1;
        } else if (value == QLatin1String("Moderate")) {
            severity = 2;
        } else if (value == QLatin1String("Severe")) {
            severity = 3;
        }
        msg.insert("severity", severity);

//        // categories
//        const auto categories = msg.take("category").toArray();
//        QStringList catOut;
//        for (const auto &category : categories) {
//            catOut.append(category.toString());
//        }
//        msg.insert("category", catOut.join(","));

        // parse parameters
        const auto params = msg.take("parameter").toArray();
        for (const auto &p : params) {
            const auto param = p.toObject();

            if (param.value("valueName").toString() == QLatin1String("sender_langname")) {
                msg.insert("sender", param.value("value"));
            }

            if (param.value("valueName").toString() == QLatin1String("instructionText")) {
                msg.insert("instructionText", param.value("value"));
            }
        }

        // parse areas
        QPointF currentLocation;

        if (m_gpsSource != nullptr) {
            currentLocation = QPointF(m_gpsSource->lastKnownPosition().coordinate().latitude(),
                                      m_gpsSource->lastKnownPosition().coordinate().longitude());
        }

        QJsonArray localPolygons;
        QRectF boundingRect;

        const auto areas = msg.take("area").toArray();
        for (const auto &area : areas) {
            const auto polygons = area.toObject().value(QStringLiteral("polygon")).toArray();
            int i = 0;

            for (const auto &polygon : polygons) {
                QJsonArray localPolygon;

                QPolygonF poly;

                const QStringList parts = polygon.toString().split(" ");

                for (const auto &part : parts) {
                    const auto location = part.split(",");


                    if (location.count() != 2)
                        continue;

                    const float lat = location.last().toFloat();
                    const float lon = location.first().toFloat();

                    if (lat < 45.0)
                        continue;

                    poly.append(QPointF(lat, lon));

                    // add coordinate to local polygon
                    QJsonArray coord;
                    coord.append(lon);
                    coord.append(lat);

                    localPolygon.append(coord);
                }

                if (poly.isEmpty())
                    continue;

                localPolygons.append(localPolygon);
                boundingRect = boundingRect.united(poly.boundingRect());

                if (!poly.isClosed()) {
                    poly.append(poly.first());
                }
                i++;

                // location
                for (const auto &loc : m_locations) {
                    const QPointF point(loc.lat, loc.lon);

                    bool local = poly.containsPoint(point, Qt::OddEvenFill);

                    if (!local)
                        continue;

                    msg.insert("local", local);
                    msg.insert("locationName", loc.name);

                    if (local)
                        break;
                }

                // check actual position if not local
                if (msg.value("local").toBool())
                    continue;


                if (!poly.containsPoint(currentLocation, Qt::OddEvenFill))
                    continue;

                msg.insert("local", true);
                msg.insert("locationName", "Current Position");
            }
        }

        msg.insert("polygons", localPolygons);

        // set local severity
        if (msg.value("local").toBool()) {
            m_localSeverity = qMax(m_localSeverity, severity);
        }

        // bounding rect
        QJsonObject rect;
        rect.insert("x", boundingRect.x());
        rect.insert("y", boundingRect.y());
        rect.insert("width", boundingRect.width());
        rect.insert("height", boundingRect.height());
        msg.insert("boundingBox", rect);


        // add message to buffers and do warning if local
        if (msg.value("local").toBool()) {
            m_localMessageBuffer.append(msg);
            sendNotification(msg);
        }
        m_messageBuffer.append(msg);
    }

    // apply if all requests are finished
    if (m_requestQueue.isEmpty()) {
        m_localMessages = QJsonDocument(m_localMessageBuffer).toJson(QJsonDocument::Compact);
        emit localMessagesChanged(m_localMessages);

        m_messages = QJsonDocument(m_messageBuffer).toJson(QJsonDocument::Compact);
        emit messagesChanged(m_messages);

        // cleanup m_warnings
        m_warnings.clear();
        for (const auto &v : m_localMessageBuffer) {
            const auto obj = v.toObject();
            if (!obj.value("local").toBool()) {
                continue;
            }

            m_warnings.append(obj.value("identifier").toString());
        }

        // signal local severity change
        emit localSeverityChanged();

        // reset
        m_localMessageBuffer = QJsonArray();
        m_messageBuffer = QJsonArray();
    }
}

void ServiceProvider::sendNotification(const QJsonObject &msg)
{
    const auto id = msg.value("identifier").toString();

    if (m_warnings.contains(id)) {
        return;
    }

    Notification notification;
    notification.setAppIcon("/usr/share/icons/hicolor/86x86/apps/harbour-apocalypse.png");
    notification.setAppName("Apocalypse");
    notification.setIcon(QStringLiteral("image://theme/icon-lock-warning"));
    notification.setCategory(QStringLiteral("x-apocalypse.warnings"));
    notification.setSummary(msg.value("msgType").toString());
    notification.setBody(msg.value("headline").toString());
    notification.setRemoteAction(Notification::remoteAction(
                                    QStringLiteral("default"),
                                    tr("Default"),
                                    QStringLiteral("org.nubecula.apocalypse.app"),
                                    QStringLiteral("/"),
                                    QStringLiteral("org.nubecula.apocalypse.app"),
                                    QStringLiteral("open"),
                                    QVariantList() << QJsonDocument(msg).toJson(QJsonDocument::Compact)
                                 ));

    if (m_playSound) {
        notification.setHintValue(QStringLiteral("sound-file"), QStringLiteral("/usr/share/harbour-apocalypse/sounds/siren.ogg"));
    }

    notification.setUrgency(Notification::Critical);
    notification.publish();
    connect(&notification, &Notification::clicked, &notification, &Notification::close);
}

void ServiceProvider::readServices()
{
    QFile file(":/data/services.json");

    if (!file.open(QIODevice::ReadOnly)) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("COULD NOT ACCESS RESSOURCE FILE");
#endif
        return;
    }

    QJsonParseError error{};

    auto services = QJsonDocument::fromJson(file.readAll(), &error).array();

    file.close();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
#endif
        return;
    }

    // define all services as active by default
    for (const auto &value : services) {
        const auto obj = value.toObject();
        Service service;
        service.id = obj.value("id").toString();
        service.name = obj.value("name").toString();
        service.url = obj.value("url").toString();

        m_services.append(service);
    }
}

void ServiceProvider::readSettings()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/apocalypse/apocalypse.conf";

    QSettings settings(path, QSettings::NativeFormat);
    settings.beginGroup("NOTIFY");
    setNotify(settings.value("notify", true).toBool());
    setPlaySound(settings.value("play_sound", false).toBool());
    settings.endGroup();

    settings.beginGroup("UPDATE");
    setAutoUpdate(settings.value("auto", true).toBool());
    setUpdateInterval(settings.value("interval", BackgroundActivity::FiveMinutes).toInt());
    settings.endGroup();

    settings.beginGroup("GPS");
    setGps(settings.value("enabled", false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DATA"));

    // locations
    int size = settings.beginReadArray(QStringLiteral("locations"));

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        Location location;
        location.name = settings.value(QStringLiteral("name")).toString();
        location.lat = settings.value(QStringLiteral("lat")).toFloat();
        location.lon = settings.value(QStringLiteral("lon")).toFloat();

        m_locations.append(location);
    }
    emit locationsChanged();

    settings.endArray();

    // services
    size = settings.beginReadArray(QStringLiteral("services"));
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        const QString id = settings.value(QStringLiteral("id")).toString();

        for (auto &service : m_services) {
            if (service.id != id) continue;

            service.active = settings.value(QStringLiteral("active"), true).toBool();
        }
    }
    settings.endArray();
    emit servicesChanged();

    settings.endGroup();
}

void ServiceProvider::writeSettings()
{
    const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/org.nubecula/apocalypse/apocalypse.conf";

    QSettings settings(path, QSettings::NativeFormat);
    settings.beginGroup("NOTIFY");
    settings.setValue("notify", m_notify);
    settings.setValue("play_sound", m_playSound);
    settings.endGroup();

    settings.beginGroup("UPDATE");
    settings.setValue("auto", m_autoUpdate);
    settings.setValue("interval", m_updateInterval);
    settings.endGroup();

    settings.beginGroup("GPS");
    settings.setValue("enabled", m_gps);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DATA"));

    settings.beginWriteArray(QStringLiteral("locations"));

    for (int i = 0; i < m_locations.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("name"), m_locations[i].name);
        settings.setValue(QStringLiteral("lat"), m_locations[i].lat);
        settings.setValue(QStringLiteral("lon"), m_locations[i].lon);
    }
    settings.endArray();

    settings.beginWriteArray(QStringLiteral("services"));

    for (int i = 0; i < m_services.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("id"), m_services[i].id);
        settings.setValue(QStringLiteral("active"), m_services[i].active);
    }
    settings.endArray();

    settings.endGroup();
}
