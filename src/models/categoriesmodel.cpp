#include "categoriesmodel.h"

CategoriesModel::CategoriesModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

quint32 CategoriesModel::categories() const
{
    return m_categories;
}

void CategoriesModel::setCategories(quint32 categories)
{
    if (m_categories == categories)
        return;

    beginResetModel();
    m_categoryStrings.clear();

    if (categories & Message::CategoryEnv)
        m_categoryStrings.append(tr("Environment"));

    if (categories & Message::CategoryGeo)
        m_categoryStrings.append(tr("Geology"));

    if (categories & Message::CategoryMet)
        m_categoryStrings.append(tr("Meteorology"));

    if (categories & Message::CategoryFire)
        m_categoryStrings.append(tr("Fire"));

    if (categories & Message::CategoryCBRNE)
        m_categoryStrings.append(tr("CBRNE"));

    if (categories & Message::CategoryInfra)
        m_categoryStrings.append(tr("Infrastructure"));

    if (categories & Message::CategoryOther)
        m_categoryStrings.append(tr("Other"));

    if (categories & Message::CategoryHealth)
        m_categoryStrings.append(tr("Health"));

    if (categories & Message::CategoryRescue)
        m_categoryStrings.append(tr("Rescue"));

    if (categories & Message::CategorySafety)
        m_categoryStrings.append(tr("Safety"));

    if (categories & Message::CategorySecurity)
        m_categoryStrings.append(tr("Security"));

    endResetModel();

    m_categories = categories;
    emit categoriesChanged(m_categories);
}

int CategoriesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_categoryStrings.count();
}

QVariant CategoriesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    switch (role) {
    case TitleRole:
        return m_categoryStrings.at(index.row());
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> CategoriesModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[TitleRole]    = "title";

    return roles;
}
