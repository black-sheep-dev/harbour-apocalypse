#ifndef SERVICEPROVIDER_H
#define SERVICEPROVIDER_H

#include <QDBusAbstractAdaptor>

#include <QGeoPositionInfoSource>
#include <QHash>

#include <keepalive/backgroundactivity.h>
#include <connman-qt5/networkmanager.h>

#include "apiinterface.h"
#include "location.h"
#include "service.h"

class ServiceProvider : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.nubecula.apocalypse.service")

    Q_PROPERTY(bool autoUpdate READ autoUpdate WRITE setAutoUpdate NOTIFY autoUpdateChanged)
    Q_PROPERTY(bool gps READ gps WRITE setGps NOTIFY gpsChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString localMessages READ localMessages NOTIFY localMessagesChanged)
    Q_PROPERTY(int localSeverity READ localSeverity NOTIFY localSeverityChanged)
    Q_PROPERTY(QString locations READ locations NOTIFY locationsChanged)
    Q_PROPERTY(QString mapBoxApiKey READ mapBoxApiKey NOTIFY mapBoxApiKeyChanged)
    Q_PROPERTY(QString messages READ messages NOTIFY messagesChanged)
    Q_PROPERTY(bool notify READ notify WRITE setNotify NOTIFY notifyChanged)
    Q_PROPERTY(bool offline READ offline NOTIFY offlineChanged)
    Q_PROPERTY(bool playSound READ playSound WRITE setPlaySound NOTIFY playSoundChanged)
    Q_PROPERTY(QString services READ services NOTIFY servicesChanged)
    Q_PROPERTY(int updateInterval READ updateInterval WRITE setUpdateInterval NOTIFY updateIntervalChanged)

public:
    explicit ServiceProvider(QObject *parent = nullptr);
    ~ServiceProvider();

    // properties
    bool autoUpdate() const;
    void setAutoUpdate(bool enable);

    bool gps() const;
    void setGps(bool enable);

    bool loading() const;

    const QString &localMessages() const;

    int localSeverity() const;

    QString locations() const;

    QString mapBoxApiKey() const;

    const QString &messages() const;

    bool notify() const;
    void setNotify(bool enabled);

    bool offline() const;

    bool playSound() const;
    void setPlaySound(bool enabled);

    QString services() const;

    int updateInterval() const;
    void setUpdateInterval(int interval);

signals:
    // properties
    void autoUpdateChanged();
    void gpsChanged();
    void loadingChanged();
    void localMessagesChanged(const QString&);
    void localSeverityChanged();
    void locationsChanged();
    void mapBoxApiKeyChanged();
    void messagesChanged(const QString&);
    void notifyChanged(bool);
    void offlineChanged();
    void playSoundChanged(bool);
    void activeServicesChanged();
    void servicesChanged();
    void updateIntervalChanged();

public slots:
    // locations
    bool addLocation(const QString &name, double latitude, double longitude);
    bool removeLocation(int index);
    bool updateLocation(int index, const QString &name, double latitude, double longitude);

    // services
    bool enableService(const QString &id, bool enable);

    // other
    void refresh();
    void restartApiInterface();
    void saveSettings();
    void shutdown();
    void test();

private slots:
    void onRequestFailed(const QString &url);
    void parseMessages(const QString &url, const QJsonArray &msgs);

private:
    void sendNotification(const QJsonObject &msg);

    // settings
    void readServices();
    void readSettings();
    void writeSettings();

    ApiInterface *m_api{nullptr};
    QJsonArray m_localMessageBuffer;
    QJsonArray m_messageBuffer;
    QStringList m_requestQueue;

    QList<Location> m_locations;
    QList<Service> m_services;

    QStringList m_warnings;

    QGeoPositionInfoSource *m_gpsSource{nullptr};

    BackgroundActivity *m_backgroundJob{nullptr};

    NetworkManager *m_nm{NetworkManager::instance()};

    // properties
    bool m_autoUpdate{false};
    bool m_gps{false};
    bool m_loading{false};
    QString m_localMessages{"[]"};
    int m_localSeverity{0};
    QString m_messages{"[]"};
    bool m_notify{true};
    bool m_offline{true};
    bool m_playSound{false};
    int m_updateInterval{BackgroundActivity::FiveMinutes};
};

#endif // SERVICEPROVIDER_H
