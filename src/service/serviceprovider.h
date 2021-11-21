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

class ServiceProvider : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool loading READ loading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(quint32 localMainCategories READ localMainCategories WRITE setLocalMainCategories NOTIFY localMainCategoriesChanged)
    Q_PROPERTY(quint8 localSeverity READ localSeverity WRITE setLocalSeverity NOTIFY localSeverityChanged)
    Q_PROPERTY(bool playSound READ playSound WRITE setPlaySound NOTIFY playSoundChanged)

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
    Q_INVOKABLE QString mapboxApiKey();
    Q_INVOKABLE MessageModel *messageModel();
    Q_INVOKABLE void saveSettings();
    Q_INVOKABLE ServiceModel *serviceModel();

    // properties
    bool loading() const; 
    quint32 localMainCategories() const;
    quint8 localSeverity() const;  
    bool playSound() const;

public slots:
    void refresh();

    // properties
    void setLoading(bool loading);
    void setLocalMainCategories(quint32 categories);
    void setLocalSeverity(quint8 severity);
    void setPlaySound(bool playSound);

signals:
    // properties
    void loadingChanged(bool loading);
    void localMainCategoriesChanged(quint32 categories);
    void localSeverityChanged(quint8 severity);
    void playSoundChanged();

private slots:
    void onRequestFinished(QNetworkReply *reply);

private:
    QNetworkRequest getRequest(const QString &url);
    QByteArray gunzip(const QByteArray &data);
    void notify(Message *msg);

    void readServices();

    void readSettings();
    void writeSettings();

    bool m_initialized{false};
    LocationModel *m_locationModel{new LocationModel(this)};
    QNetworkAccessManager *m_manager{new QNetworkAccessManager(this)};
    MessageHelper *m_messageHelper{nullptr};
    MessageModel *m_messageModel{new MessageModel(this)};
    QStringList m_notifications;
    ServiceModel *m_serviceModel{new ServiceModel(this)};

    // properties
    bool m_loading{false};
    quint32 m_localMainCategories{Message::CategoryNone};
    quint8 m_localSeverity{Message::SeverityUndefined};
    bool m_playSound{false};
};

#endif // SERVICEPROVIDER_H
