#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <QObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>

#include "src/models/locationmodel.h"
#include "src/models/messagemodel.h"
#include "src/models/servicemodel.h"
#include "src/tools/messagehelper.h"

class ServiceProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)

public:
    explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider() override;

    Q_INVOKABLE void initialize();
    Q_INVOKABLE LocationModel *locationModel();
    Q_INVOKABLE MessageModel *messageModel();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE ServiceModel *serviceModel();

    // properties
    bool loading() const;

public slots:
    Q_INVOKABLE void refresh();

    // properties
    void setLoading(bool loading);

signals:
    // properties
    void loadingChanged(bool loading);

private slots:
    void onRequestFinished(QNetworkReply *reply);
    void sendRequests();

private:
    QByteArray gunzip(const QByteArray &data);

    void readServices();
    void readSettings();
    void writeSettings();

    LocationModel *m_locationModel{nullptr};
    QNetworkAccessManager *m_manger{nullptr};
    MessageHelper *m_messageHelper{nullptr};
    MessageModel *m_messageModel{nullptr};
    QQueue<QNetworkRequest> m_requestQueue;
    bool m_sending{false};
    ServiceModel *m_serviceModel{nullptr};

    // properties
    bool m_loading{false};
};

#endif // SERVICEPROVIDER_H
