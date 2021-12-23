#include <QQmlApplicationEngine>

#include "../link_manager.h"

#include "link.h"


using namespace communication;

Link::Link()
    : QThread(nullptr)
    , m_link_counts(0)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
    qRegisterMetaType<Link>("Communication::Link");

    QObject::connect(this, &Link::signalInvokeWriteBytes, this, &Link::privateSlotWriteBytes);
}

Link::~Link()
{
    m_config.reset();
}

void Link::writeBytesThreadSafe(const char *bytes, int length)
{
    Q_EMIT signalInvokeWriteBytes(QByteArray(bytes, length));
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