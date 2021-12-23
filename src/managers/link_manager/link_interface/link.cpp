#include <QQmlApplicationEngine>

#include "link_manager.h"

#include "link.h"


using namespace communication;

Link::Link(SharedLinkConfigurationPtr &config)
    : QThread(nullptr)
    , m_link_counts(0)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<Link*>("communication::Link");
}

Link::~Link()
{
    m_p_config.reset();
}

void Link::writeBytesThreadSafe(const char *bytes, int length)
{
    QByteArray byteArray(bytes, length);
    m_write_bytes_mutex.lock();
    writeBytes(byteArray);
    m_write_bytes_mutex.unlock();
}

void Link::addLinkCounts()
{
    ++m_link_counts;
}

void Link::removeLinkCounts()
{
    if(m_link_counts)
    {
        --m_link_counts;
        if(0 == m_link_counts)
        {
            disconnect();
        }
    }
}

void Link::connectionRemoved()
{
    if(0 == m_link_counts)
    {
        disconnect();
    }
}