#pragma once

#include <memory>

#include <QThread>
#include <QMutexLocker>
#include <QDebug>
#include <QQmlApplicationEngine>


namespace managers { class LinkManager; }

namespace communication
{
    class Link : public QThread
    {
        Q_OBJECT

        friend class managers::LinkManager;

        signals:
            void connected();
            void disconnected();
            void bytesReceived(Link*, QByteArray&);
            void bytesSent(Link*, const QByteArray&);
            void commError(const QString&, const QString&);

        private slots:
            virtual void writeBytes(const QByteArray&) = 0;

        public:
            Q_INVOKABLE virtual void disconnect() = 0;

        public:
            virtual ~Link() {};

            virtual void setPortName(const QString name) = 0;
            virtual QString getPortName() const = 0;
            virtual bool getConnected() const = 0;

            enum class LinkType : int8_t
            {
                UNKNOWN_TYPE = -1,
                CAN,
                SERIAL,
                TCP,
                UDP,
            };

        protected:
            Link(LinkType link_type)
                : QThread(nullptr)
                , m_link_type(link_type)
            {
                QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
                qRegisterMetaType<Link*>("communication::Link");
            }

            LinkType getLinkType() const { return m_link_type; }

        private:
            LinkType m_link_type;

            virtual bool connect() = 0;
    };
    using SharedLinkPtr = std::shared_ptr<Link>;
    using WeakLinkPtr = std::weak_ptr<Link>;
}   // namespace communication