#include "messagesortfiltermodel.h"

MessageSortFilterModel::MessageSortFilterModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

void MessageSortFilterModel::showLocalOnly()
{
    setFilterRole(MessageModel::LocalRole);
    setFilterFixedString("true");
}
