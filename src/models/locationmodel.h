#ifndef LOCATIONMODEL_H
#define LOCATIONMODEL_H

#include <QAbstractListModel>

#include <src/entities/location.h>

class LocationModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum LocationRoles {
        LatitudeRole        = Qt::UserRole + 1,
        LongitudeRole,
        NameRole
    };
    Q_ENUM(LocationRoles)

    explicit LocationModel(QObject *parent = nullptr);

    Q_INVOKABLE Location *locationAt(int index);
    Q_INVOKABLE QList<Location *> locations() const;

    void addLocation(Location *location);
    Q_INVOKABLE void addLocation(const QString &name, float latitude, float longitude);
    Q_INVOKABLE void removeLocation(int index);
    void setLocations(const QList<Location *> &locations);

private:
    QList<Location *> m_locations;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // LOCATIONMODEL_H
