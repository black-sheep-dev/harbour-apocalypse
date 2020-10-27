#ifndef CATEGORIESMODEL_H
#define CATEGORIESMODEL_H

#include <QAbstractListModel>

#include "src/entities/message.h"

class CategoriesModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(quint32 categories READ categories WRITE setCategories NOTIFY categoriesChanged)

public:
    enum CategoryRoles {
        TitleRole       = Qt::UserRole + 1
    };
    Q_ENUM(CategoryRoles)

    explicit CategoriesModel(QObject *parent = nullptr);

    quint32 categories() const;

signals:
    void categoriesChanged(quint32 categories);

public slots:
    void setCategories(quint32 categories);

private:
    quint32 m_categories{0};
    QStringList m_categoryStrings;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // CATEGORIESMODEL_H
