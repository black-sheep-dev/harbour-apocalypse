#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

Message *MessageModel::messageAt(int index)
{
    if (index < 0 || index >= m_messages.count())
        return nullptr;

    return m_messages.at(index);
}

QList<Message *> MessageModel::messages() const
{
    return m_messages;
}

void MessageModel::addMessage(Message *msg)
{
    if (!msg)
        return;

    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    msg->setParent(this);
    m_messages.append(msg);
    endInsertRows();
}

void MessageModel::addMessages(const QList<Message *> &msgs)
{
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count() + msgs.count() - 1);
    for (auto *msg : msgs) {
        msg->setParent(this);
    }

    m_messages.append(msgs);
    endInsertRows();
}

void MessageModel::reset()
{
    beginResetModel();
    qDeleteAll(m_messages.begin(), m_messages.end());
    m_messages.clear();
    endResetModel();
}

void MessageModel::setMessages(const QList<Message *> &msgs)
{
    beginResetModel();
    if (!m_messages.isEmpty())
        qDeleteAll(m_messages.begin(), m_messages.end());

    for (auto *msg : msgs) {
        msg->setParent(this);
    }

    m_messages = msgs;
    endResetModel();
}

int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)

    return m_messages.count();
}

QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto *msg = m_messages.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return msg->headline();

    case CategoriesRole:
        return quint16(msg->categories());

    case ContactRole:
        return msg->contact();

    case DescriptionRole:
        return msg->description();

    case ExpiresRole:
        return msg->expires();

    case EventTitleRole:
        return msg->eventTitle();

    case HeadlineRole:
        return msg->headline();

    case IdentifierRole:
        return msg->identifier();

    case InstructionRole:
        return msg->instruction();

    case LocalRole:
        return msg->local();

    case MessageTypeRole:
        return msg->messageType();

    case SenderNameRole:
        return msg->senderName();

    case SentRole:
        return msg->sent();

    case SeverityRole:
        return msg->severity();

    case UrgencyRole:
        return msg->urgency();

    case IndexRole:
        return index.row();

    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;

    roles[CategoriesRole]           = "categories";
    roles[ContactRole]              = "contact";
    roles[DescriptionRole]          = "description";
    roles[ExpiresRole]              = "expires";
    roles[EventTitleRole]           = "event_title";
    roles[HeadlineRole]             = "headline";
    roles[IdentifierRole]           = "identitfier";
    roles[InstructionRole]          = "instruction";
    roles[LocalRole]                = "local";
    roles[MessageTypeRole]          = "message_type";
    roles[SenderNameRole]           = "sender_name";
    roles[SentRole]                 = "sent";
    roles[SeverityRole]             = "severity";
    roles[UrgencyRole]              = "urgency";
    roles[WebRole]                  = "web";
    roles[IndexRole]                = "idx";

    return roles;
}
