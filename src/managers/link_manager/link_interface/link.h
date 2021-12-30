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
            void bytesReceived(Link *link, QByteArray &data);
            void bytesSend(Link *link, QByteArray &data);

            void commError(const QString &title, const QString &error);

        private slots:
            virtual void writeBytes(const QByteArray&) = 0;

        public:
            Q_INVOKABLE virtual void disconnect() = 0;

        public:
            virtual ~Link() {};

            virtual bool getConnected() const = 0;

        protected:
            Link()
                : QThread(nullptr)
            {
                QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
                qRegisterMetaType<Link*>("communication::Link");
            }

        private:
            virtual bool connect() = 0;
    };
    using SharedLinkPtr = std::shared_ptr<Link>;
    using WeakLinkPtr = std::weak_ptr<Link>;
}   // namespace communication