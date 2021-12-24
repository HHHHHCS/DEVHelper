#include "q_serial.h"

#include "link_configuration.h"

#define LINK_SETTING_ROOT "LinkConfigurations"


using namespace communication;

LinkConfiguration::LinkConfiguration(const QString &name)
    : m_name(name)
    , m_dynamic(false)
    , m_auto_connect(false)
    , m_high_latency(false)
{

}

LinkConfiguration::LinkConfiguration(LinkConfiguration &obj)
{
    m_p_link = obj.m_p_link;
    m_name = obj.getName();
    m_dynamic = obj.getDynamic();
    m_auto_connect = obj.getAutoConnect();
    m_high_latency = obj.getHighLatency();

    Q_ASSERT(!m_name.isEmpty());
}

LinkConfiguration& LinkConfiguration::operator=(const LinkConfiguration& src)
{
    m_p_link = src.m_p_link;
    m_name = src.getName();
    m_dynamic = src.getDynamic();
    m_auto_connect = src.getAutoConnect();
    m_high_latency = src.getHighLatency();

    return *this;
}

const QString LinkConfiguration::getSettingsRoot()
{
    return QString(LINK_SETTING_ROOT);
}

LinkConfiguration* LinkConfiguration::createSettings(LinkConfiguration::LinkType type, const QString &name)
{
    LinkConfiguration* cfg = nullptr;
    switch(type)
    {
        case LinkConfiguration::LinkType::TypeSerial:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeCan:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeTcp:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeUdp:
        {
            break;
        }
    }

    return cfg;
}

LinkConfiguration* LinkConfiguration::duplicateSettings(LinkConfiguration &src)
{
    LinkConfiguration* dup = nullptr;
    switch(src.getLinkType())
    {
        case LinkConfiguration::LinkType::TypeSerial:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeCan:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeTcp:
        {
            break;
        }
        case LinkConfiguration::LinkType::TypeUdp:
        {
            break;
        }
    }

    return dup;
}