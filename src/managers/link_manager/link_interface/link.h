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
            /**
             * @brief 已连接信号
             */
            void sigConnected();

            /**
             * @brief 已断连信号
             */
            void sigDisconnected();

            /**
             * @brief 字节发送完成信号
             * @param 发送完成的字节数
             */
            void sigBytesSent(const qint64 sent_size);

            /**
             * @brief 字节接收完成信号
             * @param[in] 收到的数据
             */
            void sigBytesReceived(QByteArray&);

            /**
             * @brief 通信错误信号
             * @param[in] 连接名
             * @param[in] 错误信息字符串
             */
            void sigCommError(const QString&, const QString&);

        private slots:
            /**
             * @brief 字节发送槽
             * @param[in] 需要发送的字节
             */
            virtual void slotWriteBytes(const QByteArray&) = 0;

        public:
            enum class LinkType : int8_t
            {
                UNKNOWN_TYPE = -1,
                CAN,
                SERIAL,
                TCP,
                UDP,
            };

        public:
            virtual ~Link() override = default;

            virtual void setPortName(const QString name) = 0;
            virtual QString getPortName() const = 0;
            virtual bool getConnected() const = 0;

            virtual void disconnect() = 0;

            LinkType getLinkType() const { return m_link_type; }

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

        private:
            LinkType m_link_type;

            virtual bool connect() = 0;
    };
    using SharedPtrLink = std::shared_ptr<Link>;
    using WeakPtrLink = std::weak_ptr<Link>;
}   // namespace communication