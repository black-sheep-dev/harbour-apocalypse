#ifndef SERVICEMODEL_H
#define SERVICEMODEL_H

#include <QAbstractListModel>

#include "src/entities/service.h"

class ServiceModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ServiceRoles {
        ActiveRole      = Qt::UserRole + 1,
        IdRole,
        NameRole,
        UrlRole
    };
    Q_ENUM(ServiceRoles)

    explicit ServiceModel(QObject *parent = nullptr);
    ~ServiceModel() override;

    Q_INVOKABLE void addService(Service *service);
    Q_INVOKABLE Service *serviceAt(int index);
    Q_INVOKABLE QList<Service *> services() const;
    void setServices(const QList<Service *> &services);

private:
    QList<Service *> m_services;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // SERVICEMODEL_H
