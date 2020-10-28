#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>

#include <QHash>

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

    int localCount() const;
    quint8 localSeverity() const;
    Q_INVOKABLE Message *messageAt(int index);
    Q_INVOKABLE Message *messageByIdentifier(const QString &identifier);
    Q_INVOKABLE QList<Message *> messages() const;

    void addMessage(Message *msg);
    void addMessages(const QList<Message *> &msgs);
    void cleanup();
    void reset();
    void setMessages(const QList<Message *> &msgs);

signals:
    void localMainCategoriesChanged(quint32 categories);
    void localSeverityChanged(quint8 severity);

private:
    bool updateLocalSeverity(Message *msg);
    bool updateMessage(int idx, Message *newMsg);

    QList<Message *> m_messages;
    QHash<QString, int> m_indexes;
    quint8 m_severity{Message::SeverityUndefined};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // MESSAGEMODEL_H
