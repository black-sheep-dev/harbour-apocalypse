#ifndef APIINTERFACE_H
#define APIINTERFACE_H

#include <QObject>

#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class ApiInterface : public QObject
{
    Q_OBJECT

public:
    explicit ApiInterface(QObject *parent = nullptr);

signals:
    void messagesAvailable(const QString &url, const QJsonArray &msg);

public slots:
    void fetch(const QString &url);

private slots:
    void onRequestFinished();

private:
    QByteArray gunzip(const QByteArray &data) const;

    QNetworkAccessManager *m_manager{new QNetworkAccessManager(this)};
};

#endif // APIINTERFACE_H
