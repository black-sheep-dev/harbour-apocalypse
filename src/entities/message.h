#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

#include <QDateTime>

class Message : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint32 categories READ categories WRITE setCategories NOTIFY categoriesChanged)
    Q_PROPERTY(QString contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime expires READ expires WRITE setExpires NOTIFY expiresChanged)
    Q_PROPERTY(QString eventTitle READ eventTitle WRITE setEventTitle NOTIFY eventTitleChanged)
    Q_PROPERTY(QString headline READ headline WRITE setHeadline NOTIFY headlineChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString instruction READ instruction WRITE setInstruction NOTIFY instructionChanged)
    Q_PROPERTY(bool local READ local WRITE setLocal NOTIFY localChanged)
    Q_PROPERTY(MessageType messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)
    Q_PROPERTY(QString senderName READ senderName WRITE setSenderName NOTIFY senderNameChanged)
    Q_PROPERTY(QDateTime sent READ sent WRITE setSent NOTIFY sentChanged)
    Q_PROPERTY(Severity severity READ severity WRITE setSeverity NOTIFY severityChanged)
    Q_PROPERTY(Urgency urgency READ urgency WRITE setUrgency NOTIFY urgencyChanged)
    Q_PROPERTY(QString web READ web WRITE setWeb NOTIFY webChanged)

public:
    enum Category {
        CategoryNone            = 0x0000,
        CategoryCBRNE           = 0x0001,
        CategoryEnv             = 0x0002,
        CategoryFire            = 0x0004,
        CategoryGeo             = 0x0008,
        CategoryHealth          = 0x0010,
        CategoryInfra           = 0x0020,
        CategoryMet             = 0x0040,
        CategoryOther           = 0x0080,
        CategoryRescue          = 0x0100,
        CategorySafety          = 0x0200,
        CategorySecurity        = 0x0400
    };
    Q_ENUM(Category)
    Q_DECLARE_FLAGS(Categories, Category)

    enum MessageType {
        Unkown,
        Alert,
        Cancel,
        Update
    };
    Q_ENUM(MessageType)

    enum Severity {
        SeverityUndefined,
        SeverityMinor,
        SeverityModerate,
        SeveritySevere
    };
    Q_ENUM(Severity)

    enum Urgency {
        UrgencyUnkown,
        UrgencyImmediate
    };
    Q_ENUM(Urgency)

    explicit Message(QObject *parent = nullptr);

    // properties
    quint32 categories() const;
    QString contact() const;
    QString description() const;
    QDateTime expires() const;
    QString eventTitle() const;
    QString headline() const;
    QString identifier() const;
    QString instruction() const;
    bool local() const;
    MessageType messageType() const;
    QString senderName() const;
    QDateTime sent() const;
    Severity severity() const;
    Urgency urgency() const;
    QString web() const;

signals:
    // properties
    void categoriesChanged(quint32 categories);
    void contactChanged(const QString &contact);
    void descriptionChanged(const QString &description);
    void expiresChanged(const QDateTime &expires);
    void eventTitleChanged(const QString &event);
    void headlineChanged(const QString &headline);
    void identifierChanged(const QString &identifier);
    void instructionChanged(const QString &instruction);
    void localChanged(bool local);
    void messageTypeChanged(Message::MessageType messageType);
    void senderNameChanged(const QString &senderName);
    void sentChanged(const QDateTime &sent);
    void severityChanged(Message::Severity severity);
    void urgencyChanged(Message::Urgency urgency);
    void webChanged(const QString &web);

public slots:
    // properties
    void setCategories(quint32 categories);
    void setContact(const QString &contact);
    void setDescription(const QString &description);
    void setExpires(const QDateTime &expires);
    void setEventTitle(const QString &event);
    void setHeadline(const QString &headline);
    void setIdentifier(const QString &identifier);
    void setInstruction(const QString &instruction);
    void setLocal(bool local);
    void setMessageType(Message::MessageType messageType);
    void setSenderName(const QString &senderName);
    void setSent(const QDateTime &sent);
    void setSeverity(Message::Severity severity);
    void setUrgency(Message::Urgency urgency);
    void setWeb(const QString &web);

private:
    // properties
    quint32 m_categories{Category::CategoryNone};
    QString m_contact;
    QString m_description;
    QDateTime m_expires;
    QString m_eventTitle;
    QString m_headline;
    QString m_identifier;
    QString m_instruction;
    bool m_local{false};
    MessageType m_messageType{MessageType::Unkown};
    QString m_senderName;
    QDateTime m_sent;
    Severity m_severity{Severity::SeverityUndefined};
    Urgency m_urgency{Urgency::UrgencyUnkown};
    QString m_web;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Message::Categories)

#endif // MESSAGE_H
