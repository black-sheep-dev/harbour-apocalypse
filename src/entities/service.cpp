#include "service.h"

Service::Service(QObject *parent) :
    QObject(parent)
{

}

QString Service::id() const
{
    return m_id;
}

QString Service::name() const
{
    return m_name;
}

QString Service::url() const
{
    return m_url;
}

bool Service::active() const
{
    return m_active;
}

void Service::setId(const QString &id)
{
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(m_id);
}

void Service::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void Service::setUrl(const QString &url)
{
    if (m_url == url)
        return;

    m_url = url;
    emit urlChanged(m_url);
}

void Service::setActive(bool active)
{
    if (m_active == active)
        return;

    m_active = active;
    emit activeChanged(m_active);
}
