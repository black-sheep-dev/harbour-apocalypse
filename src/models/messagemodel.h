#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>

#include "src/entities/message.h"

class MessageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum MessageRoles {
        CategoriesRole          = Qt::UserRole + 1,
        ContactRole,
        DescriptionRole,
        ExpiresRole,
        EventTitleRole,
        HeadlineRole,
        IdentifierRole,
        InstructionRole,
        LocalRole,
        MessageTypeRole,
        SenderNameRole,
        SentRole,
        SeverityRole,
        UrgencyRole,
        WebRole,
        IndexRole
    };
    Q_ENUM(MessageRoles)

    explicit MessageModel(QObject *parent = nullptr);

    Q_INVOKABLE Message *messageAt(int index);
    Q_INVOKABLE QList<Message *> messages() const;

    void addMessage(Message *msg);
    void addMessages(const QList<Message *> &msgs);
    void reset();
    void setMessages(const QList<Message *> &msgs);

private:
    QList<Message *> m_messages;

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // MESSAGEMODEL_H
