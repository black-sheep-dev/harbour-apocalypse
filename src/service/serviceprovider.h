#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <QObject>

//static const QString APOCALYPSE_API_SEARCH_CHANNEL = QStringLiteral("https://warnung.bund.de/assets/json/suche_channel.json");

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QQueue>

#include "src/models/locationmodel.h"
#include "src/models/messagemodel.h"
#include "src/models/servicemodel.h"
#include "src/tools/messagehelper.h"

#include <keepalive/backgroundactivity.h>

class ServiceProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(quint32 localMainCategories READ localMainCategories WRITE setLocalMainCategories NOTIFY localMainCategoriesChanged)
    Q_PROPERTY(quint8 localSeverity READ localSeverity WRITE setLocalSeverity NOTIFY localSeverityChanged)
    Q_PROPERTY(quint8 updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    enum UpdateInterval {
        FiveMinutes,
        TenMinutes,
        FifteenMinutes,
        ThirtyMinutes,
        OneHour
    };
    Q_ENUM(UpdateInterval)

    explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider() override;

    Q_INVOKABLE void initialize();
    Q_INVOKABLE LocationModel *locationModel();
    Q_INVOKABLE MessageModel *messageModel();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE ServiceModel *serviceModel();

    // properties
    bool autoUpdate() const;
    bool loading() const; 
    quint32 localMainCategories() const;
    quint8 localSeverity() const; 
    quint8 updateInterval() const;

public slots:
    Q_INVOKABLE void refresh();

    // properties
    void setAutoUpdate(bool enable);
    void setLoading(bool loading);
    void setLocalMainCategories(quint32 categories);
    void setLocalSeverity(quint8 severity);
    void setUpdateInterval(quint8 interval);

signals:
    // properties
    void autoUpdateChanged(bool enabled);
    void loadingChanged(bool loading);
    void localMainCategoriesChanged(quint32 categories);
    void localSeverityChanged(quint8 severity);
    void updateIntervalChanged(quint8 interval);

private slots:
    void onRequestFinished(QNetworkReply *reply);
    void onUpdateRequest();
    void sendRequests();

private:
    QNetworkRequest getRequest(const QString &url);
    int getUpdateInterval() const;
    QByteArray gunzip(const QByteArray &data);
    void notify(Message *msg);
    void updateBackgroundActivity();

    void readServices();

    void readMessages();
    void writeMessages();

    void readSettings();
    void writeSettings();

    BackgroundActivity *m_activity{nullptr};
    bool m_initialized{false};
    LocationModel *m_locationModel{new LocationModel(this)};
    QNetworkAccessManager *m_manger{new QNetworkAccessManager(this)};
    MessageHelper *m_messageHelper{nullptr};
    MessageModel *m_messageModel{new MessageModel(this)};
    QStringList m_notifications;
    QQueue<QNetworkRequest> m_requestQueue;
    bool m_sending{false};
    ServiceModel *m_serviceModel{new ServiceModel(this)};

    // properties
    bool m_autoUpdate{false};
    bool m_loading{false};
    quint32 m_localMainCategories{Message::CategoryNone};
    quint8 m_localSeverity{Message::SeverityUndefined};
    quint8 m_updateInterval{0};

};

#endif // SERVICEPROVIDER_H
