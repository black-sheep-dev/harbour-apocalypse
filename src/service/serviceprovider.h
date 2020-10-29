#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <QObject>

static const QString APOCALYPSE_API_SEARCH_CHANNEL = QStringLiteral("https://warnung.bund.de/assets/json/suche_channel.json");

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
    Q_PROPERTY(quint32 localMainCategories READ localMainCategories WRITE setLocalMainCategories NOTIFY localMainCategoriesChanged)
    Q_PROPERTY(quint8 localSeverity READ localSeverity WRITE setLocalSeverity NOTIFY localSeverityChanged)

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
    quint32 localMainCategories() const;
    quint8 localSeverity() const;

public slots:
    Q_INVOKABLE void refresh();

    // properties
    void setLoading(bool loading);
    void setLocalMainCategories(quint32 categories);
    void setLocalSeverity(quint8 severity);

signals:
    // properties
    void loadingChanged(bool loading);
    void localMainCategoriesChanged(quint32 categories);
    void localSeverityChanged(quint8 severity);

private slots:
    void onRequestFinished(QNetworkReply *reply);
    void sendRequests();

private:
    QNetworkRequest getRequest(const QString &url);
    QByteArray gunzip(const QByteArray &data);

    void readServices();

    void readMessages();
    void writeMessages();

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
    quint32 m_localMainCategories{Message::CategoryNone};
    quint8 m_localSeverity{Message::SeverityUndefined};
};

#endif // SERVICEPROVIDER_H
