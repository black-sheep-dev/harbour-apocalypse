#ifndef MESSAGESORTFILTERMODEL_H
#define MESSAGESORTFILTERMODEL_H

#include <QSortFilterProxyModel>

#include "messagemodel.h"

class MessageSortFilterModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit MessageSortFilterModel(QObject *parent = nullptr);

    Q_INVOKABLE void showLocalOnly();
};

#endif // MESSAGESORTFILTERMODEL_H
