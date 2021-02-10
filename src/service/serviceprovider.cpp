#include "serviceprovider.h"

constexpr quint64 APOCALYPSE_MESSAGE_DB_FILE_MAGIC =  0x544f44;
constexpr quint16   APOCALYPSE_MESSAGE_DB_FILE_VERSION = 1;

#ifdef QT_DEBUG
#include <QDebug>
#endif

#include <QDataStream>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>

#include <notification.h>

#include <zlib.h>

ServiceProvider::ServiceProvider(QObject *parent) :
    QObject(parent)
{
    m_messageHelper = new MessageHelper(m_locationModel, this);

    connect(m_manger, &QNetworkAccessManager::finished, this, &ServiceProvider::onRequestFinished);
    connect(m_messageModel, &MessageModel::localSeverityChanged, this, &ServiceProvider::setLocalSeverity);
    connect(m_messageModel, &MessageModel::localMainCategoriesChanged, this, &ServiceProvider::setLocalMainCategories);

    readServices();
    readSettings();
    //readMessages();
}

ServiceProvider::~ServiceProvider()
{
    writeSettings();
}

void ServiceProvider::initialize()
{
    // get geocode configuration
    //m_requestQueue.enqueue(getRequest(APOCALYPSE_API_SEARCH_CHANNEL));

    refresh();

    m_initialized = true;
    updateBackgroundActivity();
}

LocationModel *ServiceProvider::locationModel()
{
    return m_locationModel;
}

MessageModel *ServiceProvider::messageModel()
{
    return m_messageModel;
}

void ServiceProvider::saveSettings()
{
    writeSettings();
}

ServiceModel *ServiceProvider::serviceModel()
{
    return m_serviceModel;
}

bool ServiceProvider::autoUpdate() const
{
    return m_autoUpdate;
}

bool ServiceProvider::loading() const
{
    return m_loading;
}

quint32 ServiceProvider::localMainCategories() const
{
    return m_localMainCategories;
}

quint8 ServiceProvider::localSeverity() const
{
    return m_localSeverity;
}

quint8 ServiceProvider::updateInterval() const
{
    return m_updateInterval;
}

void ServiceProvider::refresh()
{
    setLoading(true);

    m_messageModel->reset();


    //readMessages();

    for (const Service *service : m_serviceModel->services()) {
        if (!service->active())
            continue;

        m_requestQueue.enqueue(getRequest(service->url()));
    }

    sendRequests();
}

void ServiceProvider::setAutoUpdate(bool enable)
{
    if (m_autoUpdate == enable)
        return;

    m_autoUpdate = enable;
    emit autoUpdateChanged(m_autoUpdate);

    // functionality
    if (m_autoUpdate) {
        if (!m_activity) {
            m_activity = new BackgroundActivity(this);
            connect(m_activity, &BackgroundActivity::running, this, &ServiceProvider::onUpdateRequest);
        }
    } else {
        m_activity->stop();
        m_activity->deleteLater();
        return;
    }
}

void ServiceProvider::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
}

void ServiceProvider::setLocalMainCategories(quint32 categories)
{
    if (m_localMainCategories == categories)
        return;

    m_localMainCategories = categories;
    emit localMainCategoriesChanged(m_localMainCategories);
}

void ServiceProvider::setLocalSeverity(quint8 severity)
{
    if (m_localSeverity == severity)
        return;

    m_localSeverity = severity;
    emit localSeverityChanged(m_localSeverity);
}

void ServiceProvider::setUpdateInterval(quint8 interval)
{
    if (m_updateInterval == interval)
        return;

    m_updateInterval = interval;
    emit updateIntervalChanged(m_updateInterval);

    updateBackgroundActivity();
}

void ServiceProvider::onRequestFinished(QNetworkReply *reply)
{
    // parse reply
    if (!reply)
        return;

    if (reply->error()) {
#ifdef QT_DEBUG
        qDebug() << reply->errorString();
#endif
        reply->deleteLater();

        // send new request
        QTimer::singleShot(500, this, &ServiceProvider::sendRequests);
        return;
    }

    const QString url = reply->url().toString();
    const QByteArray raw = reply->readAll();

    QByteArray data = gunzip(raw);
    if (data.isEmpty())
        data = raw;

    QJsonParseError error{};

    const QJsonDocument doc = QJsonDocument::fromJson(data, &error);

    reply->deleteLater();

    // send new request
    QTimer::singleShot(500, this, &ServiceProvider::sendRequests);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }

    const QJsonArray array = doc.array();

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("JSON ARRAY ITEMS: ") << array.count();
#endif

    QList<Message *> messages;
    for (const QJsonValue &item : array) {
        auto *msg = new Message;
        bool ok = m_messageHelper->parseMessage(item.toObject(), msg);

        if (!ok) {
            msg->deleteLater();
            continue;
        }

        if (msg->local()) {
            notify(msg);
        }

        messages.append(msg);
    }

#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Parsed Messages: ") << messages.count();
#endif

    m_messageModel->addMessages(messages);
}

void ServiceProvider::onUpdateRequest()
{
#ifdef QT_DEBUG
    qDebug() << QStringLiteral("Background Job Update");
#endif

    if (!m_activity)
        return;

    refresh();

    m_activity->wait(getUpdateInterval());
}

void ServiceProvider::sendRequests()
{
    if (m_requestQueue.isEmpty()) {
        m_sending = false;
        m_messageModel->cleanup();
        //writeMessages();
        setLoading(false);
        return;
    }

    m_sending = true;
    m_manger->get(m_requestQueue.dequeue());
}

QNetworkRequest ServiceProvider::getRequest(const QString &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip");
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    return request;
}

int ServiceProvider::getUpdateInterval() const
{
    int interval{0};

    switch (m_updateInterval) {
    case FiveMinutes:
        interval = BackgroundActivity::FiveMinutes;
        break;

    case TenMinutes:
        interval = BackgroundActivity::TenMinutes;
        break;

    case FifteenMinutes:
        interval = BackgroundActivity::FifteenMinutes;
        break;

    case ThirtyMinutes:
        interval = BackgroundActivity::ThirtyMinutes;
        break;

    case OneHour:
        interval = BackgroundActivity::OneHour;
        break;

    default:
        interval = BackgroundActivity::FifteenMinutes;
        break;
    }

    return interval;
}

QByteArray ServiceProvider::gunzip(const QByteArray &data)
{
    if (data.size() <= 4) {
        return QByteArray();
    }

    QByteArray result;

    int ret;
    z_stream strm;
    static const int CHUNK_SIZE = 1024;
    char out[CHUNK_SIZE];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = data.size();
    strm.next_in = (Bytef*)(data.data());

    ret = inflateInit2(&strm, 15 +  32); // gzip decoding
    if (ret != Z_OK)
        return QByteArray();

    // run inflate()
    do {
        strm.avail_out = CHUNK_SIZE;
        strm.next_out = (Bytef*)(out);

        ret = inflate(&strm, Z_NO_FLUSH);
        Q_ASSERT(ret != Z_STREAM_ERROR);  // state not clobbered

        switch (ret) {
        case Z_NEED_DICT:
            ret = Z_DATA_ERROR;     // and fall through
        case Z_DATA_ERROR:
        case Z_MEM_ERROR:
            (void)inflateEnd(&strm);
            return QByteArray();
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}

void ServiceProvider::notify(Message *msg)
{
    if (m_notifications.contains(msg->identifier()))
        return;

    Notification notification;
    notification.setAppName(tr("Apocalypse"));
    notification.setIcon(QStringLiteral("image://theme/icon-lock-warning"));
    notification.setCategory(QStringLiteral("x-apocalypse.warnings"));
    notification.setSummary(tr("Warning"));
    notification.setBody(msg->headline());
    notification.setRemoteAction(Notification::remoteAction(
                                    QStringLiteral("default"),
                                    tr("Default"),
                                    QStringLiteral("harbour.apocalypse.service"),
                                    QStringLiteral("/harbour/apocalypse/service"),
                                    QStringLiteral("herbour.apocalypse.service"),
                                    QStringLiteral("open")
                                 ));
    notification.publish();
    connect(&notification, &Notification::clicked, &notification, &Notification::close);

    m_notifications.append(msg->identifier());
}

void ServiceProvider::updateBackgroundActivity()
{
    if (!m_initialized)
        return;

    if (!m_activity)
        return;

    if (m_activity->state() == BackgroundActivity::Waiting)
        m_activity->stop();

    m_activity->wait(getUpdateInterval());
}

void ServiceProvider::readServices()
{
    QFile file(":/content/services.json");

    if (!file.open(QIODevice::ReadOnly)) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("COULD NOT ACCESS RESSOURCE FILE");
#endif
        return;
    }

    QJsonParseError error{};

    const QJsonArray array = QJsonDocument::fromJson(file.readAll(), &error).array();

    file.close();

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
#endif
        return;
    }

    QList<Service *> services;
    for (const auto &item : array) {
        const QJsonObject obj = item.toObject();

        auto *service = new Service;
        service->setId(obj.value(QStringLiteral("id")).toString());
        service->setName(obj.value(QStringLiteral("name")).toString());
        service->setUrl(obj.value(QStringLiteral("url")).toString());

        services.append(service);
    }

    m_serviceModel->setServices(services);
}

void ServiceProvider::readMessages()
{
    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/messages.db"));

    if (!file.open(QIODevice::ReadOnly)) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("Could not write messages to database");
#endif
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_5_6);

    quint64 magic;
    in >> magic;

    if (magic != APOCALYPSE_MESSAGE_DB_FILE_MAGIC) {
        file.close();
        return;
    }

    quint16 version;
    in >> version;

    int count;
    in >> count;

    QList<Message *> msgs;

    for (int i = 0; i < count; ++i) {
        auto *msg = new Message;
        msg->setFromLocalStorage(true);

        QString str;
        QDateTime timestamp;

        quint32 categories;
        in >> categories;
        msg->setCategories(categories);

        in >> str;
        msg->setContact(str);

        in >> str;
        msg->setDescription(str);

        in >> timestamp;
        msg->setExpires(timestamp);

        in >> str;
        msg->setEventTitle(str);

        in >> str;
        msg->setHeadline(str);

        in >> str;
        msg->setIdentifier(str);

        in >> str;
        msg->setInstruction(str);

        bool local;
        in >> local;
        msg->setLocal(local);

        quint8 messageType;
        in >> messageType;
        msg->setMessageType(messageType);

        in >> str;
        msg->setSenderName(str);

        in >> timestamp;
        msg->setSent(timestamp);

        quint8 severity;
        in >> severity;
        msg->setSeverity(severity);

        quint8 urgency;
        in >> urgency;
        msg->setUrgency(urgency);

        in >> str;
        msg->setWeb(str);

        in >> timestamp;
        msg->setNotified(timestamp);

        msgs.append(msg);
    }

    m_messageModel->setMessages(msgs);

    file.close();
}

void ServiceProvider::writeMessages()
{
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));

    QFile file(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + QStringLiteral("/messages.db"));

    if (!file.open(QIODevice::WriteOnly)) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("Could not write messages to database");
#endif
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_5_6);

    out << APOCALYPSE_MESSAGE_DB_FILE_MAGIC;
    out << APOCALYPSE_MESSAGE_DB_FILE_VERSION;

    out << m_messageModel->localCount();

    for (const auto *msg : m_messageModel->messages()) {
        if (!msg->local())
            continue;

        out << msg->categories();
        out << msg->contact();
        out << msg->description();
        out << msg->expires();
        out << msg->eventTitle();
        out << msg->headline();
        out << msg->identifier();
        out << msg->instruction();
        out << msg->local();
        out << msg->messageType();
        out << msg->senderName();
        out << msg->sent();
        out << msg->severity();
        out << msg->urgency();
        out << msg->web();

        out << msg->notified();
    }

    file.close();
}

void ServiceProvider::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP"));
    setAutoUpdate(settings.value(QStringLiteral("auto_update"), false).toBool());
    setUpdateInterval(quint8(settings.value(QStringLiteral("update_interval"), 0).toInt()));
    settings.endGroup();

    int size{0};

    settings.beginGroup(QStringLiteral("DATA"));

    // locations
    size = settings.beginReadArray(QStringLiteral("locations"));
    QList<Location *> locations;

    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        auto *location = new Location;
        location->setName(settings.value(QStringLiteral("name")).toString());
        location->setLatitude(settings.value(QStringLiteral("lat")).toFloat());
        location->setLongitude(settings.value(QStringLiteral("lon")).toFloat());

        locations.append(location);
    }

    m_locationModel->setLocations(locations);
    settings.endArray();

    // services
    size = settings.beginReadArray(QStringLiteral("services"));
    for (int i = 0; i < size; ++i) {
        settings.setArrayIndex(i);

        const QString id = settings.value(QStringLiteral("id")).toString();

        auto *service = m_serviceModel->serviceById(id);

        if (!service)
            continue;

        service->setActive(settings.value(QStringLiteral("active")).toBool());
    }

    settings.endArray();
    settings.endGroup();
}

void ServiceProvider::writeSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("APP"));
    settings.setValue(QStringLiteral("auto_update"), m_autoUpdate);
    settings.setValue(QStringLiteral("update_interval"), m_updateInterval);
    settings.endGroup();

    settings.beginGroup(QStringLiteral("DATA"));
    settings.beginWriteArray(QStringLiteral("locations"));
    const QList<Location *> locations = m_locationModel->locations();

    for (int i = 0; i < locations.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("name"), locations.at(i)->name());
        settings.setValue(QStringLiteral("lat"), locations.at(i)->latitude());
        settings.setValue(QStringLiteral("lon"), locations.at(i)->longitude());
    }
    settings.endArray();

    settings.beginWriteArray(QStringLiteral("services"));
    const QList<Service *> services = m_serviceModel->services();

    for (int i = 0; i < services.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue(QStringLiteral("id"), services.at(i)->id());
        settings.setValue(QStringLiteral("active"), services.at(i)->active());
    }
    settings.endArray();
    settings.endGroup();
}
