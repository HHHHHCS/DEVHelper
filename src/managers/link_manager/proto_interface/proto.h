﻿#pragma once

#include <memory>

#include <QThread>
#include <QQmlApplicationEngine>


namespace managers { class LinkManager; }

namespace communication
{
    class Proto : public QThread
    {
        Q_OBJECT

        friend class managers::LinkManager;

        signals:
            /**
             * @brief 握手完成信号
             */
            void sigShakehand();

            /**
             * @brief 终止信号
             */
            void sigBreakdown();

            /**
             * @brief 协议封装信号
             */
            void sigPack(const QByteArray&);

            /**
             * @brief 协议解析信号
             */
            void sigParse(const QByteArray&);

            /**
             * @brief 协议解析完成信号
             */
            void sigParsed(const QByteArray&);

            /**
             * @brief 协议封装完成信号
             * @note 对应槽 communication::Link::slotWriteBytes
             */
            void sigPacked(const QByteArray&);

        private slots:
            /**
             * @brief 协议解析槽
             * @note 在槽内完成协议解析并进行其他处理
             */
            virtual void slotParseProto(const QByteArray& data) = 0;

            /**
             * @brief 协议封装槽
             * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
             */
            virtual void slotPackProto(const QByteArray& msg) = 0;

        public:
            enum class ProtoType : int8_t
            {
                UNKNOWN_TYPE = -1,
                MAVLINK,
                DEVLINK2,
                // TODO(huangchsh): 后续协议类型在此添加
            };

        public:
            virtual ~Proto() = default;

            QString getProtoName() const noexcept { return m_proto_name; }
            ProtoType getProtoType() const noexcept { return m_proto_type; }
            uint8_t getProtoVersion() const noexcept { return m_proto_version; }

        protected:
            explicit Proto(const ProtoType& proto_type, const QString& proto_name, const uint8_t proto_version)
                : QThread(nullptr)
                , m_proto_type(proto_type)
                , m_proto_name(proto_name)
                , m_proto_version(proto_version)
            {
                QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
                qRegisterMetaType<Proto*>("communication::Proto");
            }
            Proto(const Proto&) = delete;
            Proto(const Proto&&) = delete;
            Proto& operator=(const Proto&) = delete;
            Proto& operator=(const Proto&&) = delete;

        private:
            const ProtoType m_proto_type;
            const QString m_proto_name;
            const uint8_t m_proto_version;
    };
    using UniquePtrProto = std::unique_ptr<Proto>;
    using SharedPtrProto = std::shared_ptr<Proto>;
    using WeakPtrProto = std::weak_ptr<Proto>;
}   // namespace communication