#pragma once

#include <QThread>
#include <QMutexLocker>
#include <QDebug>
#include <QQmlApplicationEngine>

#include <memory>


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

        private Q_SLOTS:
            virtual void writeBytes(const QByteArray&) = 0;

        public:
            Q_INVOKABLE virtual void disconnect() = 0;

        public:
            virtual ~Link();

            virtual bool getConnected() const = 0;
            virtual bool getFindPortsListFlag() const = 0;
            virtual QStringList getPortNameList() const = 0;

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