#include "messagesortfiltermodel.h"

MessageSortFilterModel::MessageSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

void MessageSortFilterModel::showLocalOnly()
{
    setFilterRole(MessageModel::LocalRole);
    setFilterFixedString("true");
}
