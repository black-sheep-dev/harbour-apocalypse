#ifndef MESSAGE_H
#define MESSAGE_H

#include <QObject>

#include <QDateTime>

class Message : public QObject
{
    Q_OBJECT

    // data properties
    Q_PROPERTY(quint32 categories READ categories WRITE setCategories NOTIFY categoriesChanged)
    Q_PROPERTY(QString contact READ contact WRITE setContact NOTIFY contactChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QDateTime expires READ expires WRITE setExpires NOTIFY expiresChanged)
    Q_PROPERTY(QString eventTitle READ eventTitle WRITE setEventTitle NOTIFY eventTitleChanged)
    Q_PROPERTY(QString headline READ headline WRITE setHeadline NOTIFY headlineChanged)
    Q_PROPERTY(QString identifier READ identifier WRITE setIdentifier NOTIFY identifierChanged)
    Q_PROPERTY(QString instruction READ instruction WRITE setInstruction NOTIFY instructionChanged)
    Q_PROPERTY(bool local READ local WRITE setLocal NOTIFY localChanged)
    Q_PROPERTY(QString locationName READ locationName WRITE setLocationName NOTIFY locationNameChanged)
    Q_PROPERTY(quint8 messageType READ messageType WRITE setMessageType NOTIFY messageTypeChanged)
    Q_PROPERTY(QString senderName READ senderName WRITE setSenderName NOTIFY senderNameChanged)
    Q_PROPERTY(QDateTime sent READ sent WRITE setSent NOTIFY sentChanged)
    Q_PROPERTY(quint8 severity READ severity WRITE setSeverity NOTIFY severityChanged)
    Q_PROPERTY(quint8 urgency READ urgency WRITE setUrgency NOTIFY urgencyChanged)
    Q_PROPERTY(QString web READ web WRITE setWeb NOTIFY webChanged)

    // functional properties
    Q_PROPERTY(bool fromLocalStorage READ fromLocalStorage WRITE setFromLocalStorage NOTIFY fromLocalStorageChanged)
    Q_PROPERTY(QDateTime notified READ notified WRITE setNotified NOTIFY notifiedChanged)

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

    // data properties
    quint32 categories() const;
    QString contact() const;
    QString description() const;
    QDateTime expires() const;
    QString eventTitle() const;
    QString headline() const;
    QString identifier() const;
    QString instruction() const;
    bool local() const;
    QString locationName() const;
    quint8 messageType() const;
    QString senderName() const;
    QDateTime sent() const;
    quint8 severity() const;
    quint8 urgency() const;
    QString web() const;

    // functional properties
    bool fromLocalStorage() const;
    QDateTime notified() const;

signals:
    // data properties
    void categoriesChanged(quint32 categories);
    void contactChanged(const QString &contact);
    void descriptionChanged(const QString &description);
    void expiresChanged(const QDateTime &expires);
    void eventTitleChanged(const QString &event);
    void headlineChanged(const QString &headline);
    void identifierChanged(const QString &identifier);
    void instructionChanged(const QString &instruction);
    void localChanged(bool local);
    void locationNameChanged(const QString &name);
    void messageTypeChanged(quint8 messageType);
    void senderNameChanged(const QString &senderName);
    void sentChanged(const QDateTime &sent);
    void severityChanged(quint8 severity);
    void urgencyChanged(quint8 urgency);
    void webChanged(const QString &web);

    // functional properties
    void idChanged(quint32 id);
    void fromLocalStorageChanged(bool fromLocalStorage);
    void notifiedChanged(const QDateTime &notified);  

public slots:
    // data properties
    void setCategories(quint32 categories);
    void setContact(const QString &contact);
    void setDescription(const QString &description);
    void setExpires(const QDateTime &expires);
    void setEventTitle(const QString &event);
    void setHeadline(const QString &headline);
    void setIdentifier(const QString &identifier);
    void setInstruction(const QString &instruction);
    void setLocal(bool local);
    void setLocationName(const QString &name);
    void setMessageType(quint8 messageType);
    void setSenderName(const QString &senderName);
    void setSent(const QDateTime &sent);
    void setSeverity(quint8 severity);
    void setUrgency(quint8 urgency);
    void setWeb(const QString &web);

    // functional properties
    void setFromLocalStorage(bool fromLocalStorage);
    void setNotified(const QDateTime &notified);

private:
    // data properties
    quint32 m_categories{Category::CategoryNone};
    QString m_contact;
    QString m_description;
    QDateTime m_expires;
    QString m_eventTitle;
    QString m_headline;
    QString m_identifier;
    QString m_instruction;
    bool m_local{false};
    QString m_locationName;
    quint8 m_messageType{MessageType::Unkown};
    QString m_senderName;
    QDateTime m_sent;
    quint8 m_severity{Severity::SeverityUndefined};
    quint8 m_urgency{Urgency::UrgencyUnkown};
    QString m_web;

    // functional properties
    bool m_fromLocalStorage{false};
    QDateTime m_notified{QDateTime()};
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Message::Categories)

#endif // MESSAGE_H
