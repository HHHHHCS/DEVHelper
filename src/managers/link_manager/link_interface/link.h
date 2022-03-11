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
            void sigConnected();
            void sigDisconnected();
            void sigBytesReceived(Link*, QByteArray&);
            void sigBytesSent(Link*, const qint64);
            void sigCommError(const QString&, const QString&);

        private slots:
            virtual void slotWriteBytes(const QByteArray&) = 0;

        public:
            Q_INVOKABLE virtual void disconnect() = 0;

        public:
            virtual ~Link() override = default;

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
            explicit Link(LinkType link_type)
                : QThread(nullptr)
                , m_link_type(link_type)
            {
                QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
                qRegisterMetaType<Link*>("communication::Link");
            }
            Link(const Link&) = delete;
            Link(const Link&&) = delete;
            Link& operator=(const Link&) = delete;
            Link& operator=(const Link&&) = delete;

            LinkType getLinkType() const { return m_link_type; }

        private:
            LinkType m_link_type;

            virtual bool connect() = 0;
    };
    using SharedPtrLink = std::shared_ptr<Link>;
    using WeakPtrLink = std::weak_ptr<Link>;
}   // namespace communication