#include "message.h"

Message::Message(QObject *parent) :
    QObject(parent)
{

}

quint32 Message::categories() const
{
    return m_categories;
}

QString Message::contact() const
{
    return m_contact;
}

QString Message::description() const
{
    return m_description;
}

QDateTime Message::expires() const
{
    return m_expires;
}

QString Message::eventTitle() const
{
    return m_eventTitle;
}

QString Message::headline() const
{
    return m_headline;
}

QString Message::identifier() const
{
    return m_identifier;
}

QString Message::instruction() const
{
    return m_instruction;
}

bool Message::local() const
{
    return m_local;
}

quint8 Message::messageType() const
{
    return m_messageType;
}

QString Message::senderName() const
{
    return m_senderName;
}

QDateTime Message::sent() const
{
    return m_sent;
}

quint8 Message::severity() const
{
    return m_severity;
}

quint8 Message::urgency() const
{
    return m_urgency;
}

QString Message::web() const
{
    return m_web;
}

bool Message::fromLocalStorage() const
{
    return m_fromLocalStorage;
}

QDateTime Message::notified() const
{
    return m_notified;
}

void Message::setCategories(quint32 categories)
{
    if (m_categories == categories)
        return;

    m_categories = categories;
    emit categoriesChanged(m_categories);
}

void Message::setContact(const QString &contact)
{
    if (m_contact == contact)
        return;

    m_contact = contact;
    emit contactChanged(m_contact);
}

void Message::setDescription(const QString &description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}

void Message::setExpires(const QDateTime &expires)
{
    if (m_expires == expires)
        return;

    m_expires = expires;
    emit expiresChanged(m_expires);
}

void Message::setEventTitle(const QString &event)
{
    if (m_eventTitle == event)
        return;

    m_eventTitle = event;
    emit eventTitleChanged(m_eventTitle);
}

void Message::setHeadline(const QString &headline)
{
    if (m_headline == headline)
        return;

    m_headline = headline;
    emit headlineChanged(m_headline);
}

void Message::setIdentifier(const QString &identifier)
{
    if (m_identifier == identifier)
        return;

    m_identifier = identifier;
    emit identifierChanged(m_identifier);
}

void Message::setInstruction(const QString &instruction)
{
    if (m_instruction == instruction)
        return;

    m_instruction = instruction;
    emit instructionChanged(m_instruction);
}

void Message::setLocal(bool local)
{
    if (m_local == local)
        return;

    m_local = local;
    emit localChanged(m_local);
}

void Message::setMessageType(quint8 messageType)
{
    if (m_messageType == messageType)
        return;

    m_messageType = messageType;
    emit messageTypeChanged(m_messageType);
}

void Message::setSenderName(const QString &senderName)
{
    if (m_senderName == senderName)
        return;

    m_senderName = senderName;
    emit senderNameChanged(m_senderName);
}

void Message::setSent(const QDateTime &sent)
{
    if (m_sent == sent)
        return;

    m_sent = sent;
    emit sentChanged(m_sent);
}

void Message::setSeverity(quint8 severity)
{
    if (m_severity == severity)
        return;

    m_severity = severity;
    emit severityChanged(m_severity);
}

void Message::setUrgency(quint8 urgency)
{
    if (m_urgency == urgency)
        return;

    m_urgency = urgency;
    emit urgencyChanged(m_urgency);
}

void Message::setWeb(const QString &web)
{
    if (m_web == web)
        return;

    m_web = web;
    emit webChanged(m_web);
}

void Message::setFromLocalStorage(bool fromLocalStorage)
{
    if (m_fromLocalStorage == fromLocalStorage)
        return;

    m_fromLocalStorage = fromLocalStorage;
    emit fromLocalStorageChanged(m_fromLocalStorage);
}

void Message::setNotified(const QDateTime &notified)
{
    if (m_notified == notified)
        return;

    m_notified = notified;
    emit notifiedChanged(m_notified);
}
