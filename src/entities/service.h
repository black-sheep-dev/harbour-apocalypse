#ifndef SERVICE_H
#define SERVICE_H

#include <QObject>

class Service : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)

public:
    explicit Service(QObject *parent = nullptr);

    QString id() const;
    QString name() const;
    QString url() const;
    bool active() const;

signals:
    void idChanged(const QString &id);
    void nameChanged(const QString &name);
    void urlChanged(const QString &url);
    void activeChanged(bool active);

public slots:
    void setId(const QString &id);
    void setName(const QString &name);
    void setUrl(const QString &url);
    void setActive(bool active);

private:
    bool m_active{true};
    QString m_id;
    QString m_name;
    QString m_url;

};

#endif // SERVICE_H
