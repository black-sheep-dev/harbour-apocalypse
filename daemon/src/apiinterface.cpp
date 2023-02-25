#include "apiinterface.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkRequest>

#include <zlib.h>

ApiInterface::ApiInterface(QObject *parent) : QObject(parent)
{

}

void ApiInterface::fetch(const QString &url)
{
    QNetworkRequest request(url);
    request.setRawHeader("Cache-Control", "no-cache");
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Accept-Encoding", "gzip");
    request.setSslConfiguration(QSslConfiguration::defaultConfiguration());

    auto reply = m_manager->get(request);
    connect(reply, &QNetworkReply::finished, this, &ApiInterface::onRequestFinished);
}

void ApiInterface::onRequestFinished()
{
    auto reply = qobject_cast<QNetworkReply *>(sender());

    if (reply == nullptr) {
        return;
    }

    if (reply->error()) {
        qCritical() << reply->errorString();
        emit requestFailed(reply->url().toString());
        reply->deleteLater();
        return;
    }

    QJsonParseError error{};

    const auto array = QJsonDocument::fromJson(gunzip(reply->readAll()), &error).array();

    reply->deleteLater();

    emit messagesAvailable(reply->url().toString(), array);

    if (error.error) {
#ifdef QT_DEBUG
        qDebug() << QStringLiteral("JSON PARSE ERROR");
        qDebug() << error.errorString();
#endif
        return;
    }
}

QByteArray ApiInterface::gunzip(const QByteArray &data) const
{
    if (data.size() <= 4) {
        return data;
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
        return data;

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
            return data;
        }

        result.append(out, CHUNK_SIZE - strm.avail_out);
    } while (strm.avail_out == 0);

    // clean up and return
    inflateEnd(&strm);
    return result;
}
