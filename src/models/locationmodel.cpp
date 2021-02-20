#include "locationmodel.h"

LocationModel::LocationModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

Location *LocationModel::locationAt(int index)
{
    if (index < 0 || index >= m_locations.count())
        return nullptr;

    return m_locations.at(0);
}

QList<Location *> LocationModel::locations() const
{
    return m_locations;
}

void LocationModel::addLocation(Location *location)
{
    if (!location)
        return;

    beginInsertRows(QModelIndex(), m_locations.count(), m_locations.count());
    location->setParent(this);
    m_locations.append(location);
    endInsertRows();
}

void LocationModel::addLocation(const QString &name, float latitude, float longitude)
{
    auto location = new Location(this);
    location->setName(name);
    location->setLatitude(latitude);
    location->setLongitude(longitude);
    addLocation(location);
}

void LocationModel::removeLocation(int index)
{
    auto location = locationAt(index);

    if (!location)
        return;

    beginRemoveRows(QModelIndex(), index, index);
    m_locations.takeAt(index)->deleteLater();
    endRemoveRows();
}

void LocationModel::setLocations(const QList<Location *> &locations)
{
    beginResetModel();
    if (!m_locations.isEmpty())
        qDeleteAll(m_locations.begin(), m_locations.end());

    for (auto location : locations) {
        location->setParent(this);
    }

    m_locations = locations;
    endResetModel();
}

int LocationModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_locations.count();
}

QVariant LocationModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    auto loc = m_locations.at(index.row());

    switch (role) {
    case NameRole:
        return loc->name();

    case LatitudeRole:
        return loc->latitude();

    case LongitudeRole:
        return loc->longitude();

    default:
        return QVariant();
    }
}

bool LocationModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;

    auto location = m_locations.at(index.row());

    switch (role) {
    case NameRole:
        location->setName(value.toString());
        break;

    case LatitudeRole:
        location->setLatitude(value.toFloat());
        break;

    case LongitudeRole:
        location->setLongitude(value.toFloat());
        break;

    default:
        return false;
    }

    emit dataChanged(index, index);

    return true;
}

QHash<int, QByteArray> LocationModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[LatitudeRole]     = "latitude";
    roles[LongitudeRole]    = "longitude";
    roles[NameRole]         = "name";

    return roles;
}
