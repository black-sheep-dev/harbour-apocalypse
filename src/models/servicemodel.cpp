#include "servicemodel.h"

ServiceModel::ServiceModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

ServiceModel::~ServiceModel()
{
    qDeleteAll(m_services.begin(), m_services.end());
}

void ServiceModel::addService(Service *service)
{
    if (!service)
        return;

    beginInsertRows(QModelIndex(), m_services.count(), m_services.count());
    m_services.append(service);
    endInsertRows();
}

Service *ServiceModel::serviceAt(int index)
{
    if (index < 0 || index >= m_services.count())
        return nullptr;

    return m_services.at(index);
}

Service *ServiceModel::serviceById(const QString &id)
{
    for (auto *service : m_services) {
        if (service->id() == id)
            return service;
    }

    return nullptr;
}

QList<Service *> ServiceModel::services() const
{
    return m_services;
}

void ServiceModel::setServices(const QList<Service *> &services)
{
    beginResetModel();
    qDeleteAll(m_services.begin(), m_services.end());
    m_services.clear();
    m_services = services;
    endResetModel();
}

int ServiceModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_services.count();
}

QVariant ServiceModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto *service = m_services.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return service->name();

    case ActiveRole:
        return service->active();

    case IdRole:
        return service->id();

    case NameRole:
        return service->name();

    case UrlRole:
        return service->url();

    default:
        return QVariant();
    }
}

bool ServiceModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto *service = m_services.at(index.row());

    switch (role) {
    case ActiveRole:
        service->setActive(value.toBool());
        break;

    case IdRole:
        service->setId(value.toString());
        break;

    case NameRole:
        service->setName(value.toString());
        break;

    case UrlRole:
        service->setUrl(value.toString());
        break;

    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

QHash<int, QByteArray> ServiceModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[ActiveRole]       = "active";
    roles[IdRole]           = "service_id";
    roles[NameRole]         = "name";
    roles[UrlRole]          = "url";

    return roles;
}
