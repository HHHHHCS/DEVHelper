#pragma once

#include <memory>

#include <QStringList>

#include "devlink2.h"

#include "proto.h"


namespace communication
{
    class Devlink2Proto : public Proto
    {
        Q_OBJECT

        private slots:
            /**
             * @brief 协议解析槽
             * @note 在槽内完成协议解析并进行其他处理
             */
            void slotParseProto(const QByteArray&) override;

            /**
             * @brief 协议封装槽
             * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
             */
            void slotPackProto(const QByteArray&) override;

        public:
            Devlink2Proto(const SharedPtrLink& link, bool do_self_thread = true);
            ~Devlink2Proto();
            Devlink2Proto(const Devlink2Proto&) = delete;
            Devlink2Proto(const Devlink2Proto&&) = delete;
            Devlink2Proto& operator=(const Devlink2Proto&) = delete;
            Devlink2Proto& operator=(const Devlink2Proto&&) = delete;

        private:
            using UniquePtrDevlink2 = std::unique_ptr<devconn::Devlink2>;
            using SharedPtrDevlink2 = std::shared_ptr<devconn::Devlink2>;
            using WeakPtrDevlink2 = std::weak_ptr<devconn::Devlink2>;

        private:
            const SharedPtrLink m_bind_link;

            UniquePtrDevlink2 m_proto_ptr;
    };
}   // namespace communication