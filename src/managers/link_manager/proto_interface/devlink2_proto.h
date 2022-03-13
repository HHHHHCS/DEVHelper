#pragma once

#include <QMap>
#include <QStringList>
#include <QMutexLocker>

#include "devlink2.h"

#include "proto.h"


namespace communication
{
    class Devlink2Proto : public Proto
    {
        Q_OBJECT

        signals:
            /**
             * @brief 触发恢复出厂成功信号
             */
            void sigRestoreFactorySucceed();

        private slots:
            /**
             * @brief 协议解析槽
             * @note 在槽内完成协议解析并进行其他处理
             * @param[in] data 待解析数据
             */
            void slotParseProto(const QByteArray& data) override;

            /**
             * @brief 协议封装槽
             * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
             * @param[in] msg_id 消息ID
             * @param[in] msg   消息数据
             */
            void slotPackProto(uint32_t msg_id, const QByteArray& data) override;

        public:
            explicit Devlink2Proto(const QString& name, const awlink_channel_t chan);
            Devlink2Proto(const Devlink2Proto&) = delete;
            Devlink2Proto(const Devlink2Proto&&) = delete;
            Devlink2Proto& operator=(const Devlink2Proto&) = delete;
            Devlink2Proto& operator=(const Devlink2Proto&&) = delete;
            ~Devlink2Proto() override = default;

        private:
            using UniquePtrDevlink2 = std::unique_ptr<devconn::Devlink2>;

        private:
            UniquePtrDevlink2 m_proto_ptr;

            QMutex m_parse_mutex;
            QMutex m_pack_mutex;
    };
    using UniquePtrDevlink2Proto = std::unique_ptr<Devlink2Proto>;
}   // namespace communication