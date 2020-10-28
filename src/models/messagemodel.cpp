#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int MessageModel::localCount() const
{
    int count = 0;

    for (const auto *msg : m_messages) {
        if (msg->local())
            count++;
    }

    return count;
}

quint8 MessageModel::localSeverity() const
{
    return m_severity;
}

Message *MessageModel::messageAt(int index)
{
    if (index < 0 || index >= m_messages.count())
        return nullptr;

    return m_messages.at(index);
}

Message *MessageModel::messageByIdentifier(const QString &identifier)
{
    int idx = m_indexes.value(identifier, -1);

    if (idx < 0 || idx >= m_messages.count())
        return nullptr;

    return m_messages.at(idx);
}

QList<Message *> MessageModel::messages() const
{
    return m_messages;
}

void MessageModel::addMessage(Message *msg)
{
    if (!msg)
        return;

    msg->setParent(this);

    // update local severity
    if (msg->local())
        updateLocalSeverity(msg);

    // update
    int idx = m_indexes.value(msg->identifier(), -1);

    if (updateMessage(idx, msg))
        return;

    // add if not exist
    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count());
    m_indexes.insert(msg->identifier(), m_messages.count());
    m_messages.append(msg);
    endInsertRows();
}

void MessageModel::addMessages(const QList<Message *> &msgs)
{
    QList<Message *> newMsgs;

    for (auto *msg : msgs) {
        msg->setParent(this);

        // update local severity
        if (msg->local())
            updateLocalSeverity(msg);

        // update
        int index = m_indexes.value(msg->identifier(), -1);

        if (updateMessage(index, msg))
            continue;

        // add if not exist
        newMsgs.append(msg);
    }

    if (newMsgs.isEmpty())
        return;

    beginInsertRows(QModelIndex(), m_messages.count(), m_messages.count() + msgs.count() - 1);
    int idx = messages().count();

    for (auto *msg : newMsgs) {
        m_indexes.insert(msg->identifier(), idx);
        idx++;
    }

    m_messages.append(newMsgs);
    endInsertRows();
}

void MessageModel::cleanup()
{
    beginResetModel();

    // remove local stored messages
    for (auto *msg : m_messages) {
        if (!msg->fromLocalStorage())
            continue;

        m_messages.removeAll(msg);
    }

    // update indexes;
    m_indexes.clear();

    for (int i = 0; i < m_messages.size(); ++i) {
        m_indexes.insert(m_messages.at(i)->identifier(), i);
    }

    endResetModel();
}

void MessageModel::reset()
{
    beginResetModel();
    qDeleteAll(m_messages.begin(), m_messages.end());
    m_messages.clear();
    m_indexes.clear();
    endResetModel();
}

void MessageModel::setMessages(const QList<Message *> &msgs)
{
    beginResetModel();
    if (!m_messages.isEmpty()) {
        qDeleteAll(m_messages.begin(), m_messages.end());
        m_indexes.clear();
    }

    int idx = 0;

    for (auto *msg : msgs) {
        msg->setParent(this);
        m_indexes.insert(msg->identifier(), idx);
        idx++;
    }

    m_messages = msgs;
    endResetModel();
}

bool MessageModel::updateLocalSeverity(Message *msg)
{
    quint8 max = qMax(msg->severity(), m_severity);

    if (m_severity == max)
        return false;

    m_severity = max;
    emit localSeverityChanged(m_severity);
    emit localMainCategoriesChanged(msg->categories());
    return true;
}

bool MessageModel::updateMessage(int idx, Message *newMsg)
{
    if (idx < 0 || idx >= m_messages.count())
        return false;

    auto *old = m_messages.at(idx);
    m_messages.replace(idx, newMsg);
    old->deleteLater();

    const QModelIndex modelIdx = index(idx);
    emit dataChanged(modelIdx, modelIdx);

    return true;
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
