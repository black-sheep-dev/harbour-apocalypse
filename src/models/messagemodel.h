#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>

#include <QHash>

#include "src/entities/message.h"

class MessageModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(quint8 localSeverity READ localSeverity WRITE setLocalSeverity NOTIFY localSeverityChanged)

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
        LocationNameRole,
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
    Q_INVOKABLE Message *messageAt(int index);
    Q_INVOKABLE Message *messageByIdentifier(const QString &identifier);
    Q_INVOKABLE QList<Message *> messages() const;

    void addMessage(Message *msg);
    void addMessages(const QList<Message *> &msgs);
    void cleanup();
    void reset();
    void setMessages(const QList<Message *> &msgs);

    // properties
    quint8 localSeverity() const;

signals:
    void changed();
    void localMainCategoriesChanged(quint32 categories);

    // properties
    void localSeverityChanged(quint8 severity);

public slots:
    // properties
    void setLocalSeverity(quint8 severity);

private slots:
    void updateLocalSeverity();

private:
    bool updateMessage(int idx, Message *newMsg);

    QList<Message *> m_messages;
    QHash<QString, int> m_indexes;

    // properties
    quint8 m_localSeverity{Message::SeverityUndefined};

    // QAbstractItemModel interface
public:
    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // MESSAGEMODEL_H
