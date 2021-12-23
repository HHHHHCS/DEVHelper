#pragma once

#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QDebug>

#include <memory>

#include "Link_configuration.h"


namespace communication
{
    class LinkManager;

    class Link : public QThread
    {
        Q_SIGNALS:
            void connected();
            void disconnected();
            void bytesReceived(Link *link, QByteArray &data);
            void bytesSend(Link *link, QByteArray &data);

            void commError(const QString &title, const QString &error);

        public:
            virtual ~Link();

            virtual bool getConnected() const = 0;

            SharedLinkConfigurationPtr linkConfiguration() const { return m_p_config; }

            void writeBytesThreadSafe(const char *bytes, int length);
            void addLinkCounts();
            void removeLinkCounts();

        public:
            Q_INVOKABLE virtual void disconnect() = 0;

        protected:
            Link(SharedLinkConfigurationPtr &config);

            SharedLinkConfigurationPtr m_p_config;

            void connectionRemoved();

        private:
            mutable QMutex m_write_bytes_mutex;
            int m_link_counts;

            virtual bool connect() = 0;
            virtual void writeBytes(const QByteArray) = 0;
    };
    using SharedLinkPtr = std::shared_ptr<Link>;
    using WeakLinkPtr = std::weak_ptr<Link>;
}