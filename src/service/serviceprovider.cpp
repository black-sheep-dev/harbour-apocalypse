#include "serviceprovider.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QSettings>
#include <QStandardPaths>
#include <QTimer>

#include <zlib.h>

ServiceProvider::ServiceProvider(QObject *parent) :
    QObject(parent),
    m_locationModel(new LocationModel(this)),
    m_manger(new QNetworkAccessManager(this)),
    m_messageModel(new MessageModel(this)),
    m_serviceModel(new ServiceModel(this))
{
    m_messageHelper = new MessageHelper(m_locationModel, this);

    connect(m_manger, &QNetworkAccessManager::finished, this, &ServiceProvider::onRequestFinished);

    readServices();
    readSettings();
}

ServiceProvider::~ServiceProvider()
{
    writeSettings();
}

void ServiceProvider::initialize()
{
    refresh();
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

bool ServiceProvider::loading() const
{
    return m_loading;
}

void ServiceProvider::refresh()
{
    setLoading(true);

    m_messageModel->reset();


    for (const Service *service : m_serviceModel->services()) {
        if (!service->active())
            continue;

        QNetworkRequest request(service->url());
        request.setRawHeader("Cache-Control", "no-cache");
        request.setRawHeader("Accept", "application/json");
        request.setRawHeader("Connection", "keep-alive");
        request.setRawHeader("Accept-Encoding", "gzip");
        request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

        m_requestQueue.enqueue(request);
    }

    sendRequests();
}

void ServiceProvider::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    m_loading = loading;
    emit loadingChanged(m_loading);
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
        QTimer::singleShot(1000, this, &ServiceProvider::sendRequests);
        return;
    }

    const QByteArray data = gunzip(reply->readAll());

    QJsonParseError error{};

    const QJsonArray array = QJsonDocument::fromJson(data, &error).array();

    reply->deleteLater();

    // send new request
    QTimer::singleShot(1000, this, &ServiceProvider::sendRequests);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }

    QList<Message *> messages;
    for (const QJsonValue &item : array) {
        Message *msg = new Message;
        bool ok = m_messageHelper->parseMessage(item.toObject(), msg);

        if (!ok) {
            msg->deleteLater();
            continue;
        }

        messages.append(msg);
    }

    m_messageModel->addMessages(messages);
}

void ServiceProvider::sendRequests()
{
    if (m_requestQueue.isEmpty()) {
        m_sending = false;
        setLoading(false);
        return;
    }

    m_sending = true;
    m_manger->get(m_requestQueue.dequeue());
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
    for (const QJsonValue &item : array) {
        const QJsonObject obj = item.toObject();

        auto *service = new Service;
        service->setId(obj.value(QStringLiteral("id")).toString());
        service->setName(obj.value(QStringLiteral("name")).toString());
        service->setUrl(obj.value(QStringLiteral("url")).toString());

        services.append(service);
    }

    m_serviceModel->setServices(services);
}

void ServiceProvider::readSettings()
{
    QSettings settings;

    settings.beginGroup(QStringLiteral("DATA"));
    int size = settings.beginReadArray(QStringLiteral("locations"));
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
    settings.endGroup();
}

void ServiceProvider::writeSettings()
{
    QSettings settings;

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
    settings.endGroup();
}
