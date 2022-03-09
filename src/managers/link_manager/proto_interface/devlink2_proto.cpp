﻿#include <QDebug>

#include "devlink2_proto.h"


using namespace communication;
using namespace devconn;

Devlink2Proto::Devlink2Proto(const QString& name, const awlink_channel_t chan)
    : Proto(ProtoType::DEVLINK2, ("dlink2-" + name))
    , m_proto_ptr(std::make_unique<devconn::Devlink2>(getProtoName().toStdString(), chan))
{
    if(m_proto_ptr)
    {
        if(Devlink2::DEV_SUCCESS == m_proto_ptr->initDevlink())
        {
            m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_DEV_HEARTBEAT, [this](const char* msg)
            {
                // TODO(huangchsh): 消息转发
            });

            m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_QUERY_OR_MODIFY_DEV_PARAMS_ACK, [this](const char* msg)
            {
                // TODO(huangchsh): 消息转发
            });

            m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_RESTORE_FACTORY_ACK, [this](const char* msg)
            {
                // TODO(huangchsh): 消息转发
            });

            m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_UPDATE_FIRMWARE_ACK, [this](const char* msg)
            {
                // TODO(huangchsh): 消息转发
            });
        }
    }
}

/**
 * @brief 协议解析槽
 * @note 在槽内完成协议解析并进行其他处理
 * @param[in] buffer 待解析数据
 */
void Devlink2Proto::slotParseProto(const QByteArray& buffer)
{
    if(buffer.isEmpty())
    {
        return;
    }

    if(!m_proto_ptr)
    {
        return;
    }

    for(auto i : buffer)
    {
        m_proto_ptr->decodeMsg(static_cast<uint8_t>(i));
    }

    emit sigParsed(this);
}

/**
 * @brief 协议封装槽
 * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
 * @param[in] msg_id 消息ID
 * @param[in] msg   消息数据
 */
void Devlink2Proto::slotPackProto(uint32_t msg_id, const QByteArray& msg)
{
    if(msg.isEmpty())
    {
        return;
    }

    if(!m_proto_ptr)
    {
        return;
    }

    awlink_message_t tmp{0};
    m_proto_ptr->encodeMsg(msg_id, msg.data(), tmp);

    emit sigPacked(this, QByteArray(reinterpret_cast<char*>(&tmp), sizeof(tmp)));
}