#include <QDebug>

#include "devlink2_proto.h"


using namespace communication;
using namespace devconn;

Devlink2Proto::Devlink2Proto(const QString& name, const awlink_channel_t chan)
    : Proto(ProtoType::DEVLINK2, ("dlink2-" + name), (253 == AWLINK_STX ? 2 : 1))
    , m_proto_ptr(std::make_unique<devconn::Devlink2>(getProtoName().toStdString(), chan))
    , m_parse_mutex()
    , m_pack_mutex()
{
    if(m_proto_ptr)
    {
        if(Devlink2::DEV_SUCCESS == m_proto_ptr->initDevlink())
        {
            if(m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_DEV_HEARTBEAT, [this](const char* msg)
            {
                if(!msg)
                {
                    return;
                }

                uint32_t msg_id(AWLINK_MSG_ID_DEV_HEARTBEAT);
                auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
                bytes.append(msg, sizeof(awlink_dev_heartbeat_t));
                emit sigParsed(bytes);
            }))
            {
                return;
            }

            if(m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_DEV_PARAM_HOST_QUERY_OR_MODIFY_ACK, [this](const char* msg)
            {
                if(!msg)
                {
                    return;
                }

                uint32_t msg_id(AWLINK_MSG_ID_DEV_PARAM_HOST_QUERY_OR_MODIFY_ACK);
                auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
                bytes.append(msg, sizeof(awlink_dev_param_host_query_or_modify_ack_t));
                emit sigParsed(bytes);
            }))
            {
                return;
            }

            if(m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_RESTORE_FACTORY_ACK, [this](const char* msg)
            {
                if(!msg)
                {
                    return;
                }

                uint32_t msg_id(AWLINK_MSG_ID_RESTORE_FACTORY_ACK);
                auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
                bytes.append(msg, sizeof(awlink_restore_factory_ack_t));
                emit sigParsed(bytes);
            }))
            {
                return;
            }

            if(m_proto_ptr->registerMsgHandle(AWLINK_MSG_ID_UPDATE_FIRMWARE_ACK, [this](const char* msg)
            {
                if(!msg)
                {
                    return;
                }

                uint32_t msg_id(AWLINK_MSG_ID_UPDATE_FIRMWARE_ACK);
                auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
                bytes.append(msg, sizeof(awlink_update_firmware_ack_t));
                emit sigParsed(bytes);
            }))
            {
                return;
            }
        }
    }

    connect(this, &Devlink2Proto::sigPack, this, &Devlink2Proto::slotPackProto, Qt::DirectConnection);
}

Devlink2Proto::~Devlink2Proto()
{
    disconnect(this, &Devlink2Proto::sigPack, this, &Devlink2Proto::slotPackProto);
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

    QMutexLocker locker(&m_parse_mutex);
    for(auto& i : buffer)
    {
        m_proto_ptr->decodeMsg(static_cast<uint8_t>(i));
    }
}

/**
 * @brief 协议封装槽
 * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
 * @param[in] msg   消息数据。前4字节为msg_id
 */
void Devlink2Proto::slotPackProto(const QByteArray& msg)
{
    if(msg.isEmpty())
    {
        return;
    }

    if(!m_proto_ptr)
    {
        return;
    }

    QMutexLocker locker(&m_pack_mutex);
    awlink_message_t tmp{0};
    uint32_t msg_id(0);
    memcpy(&msg_id, msg.data(), sizeof(uint32_t));
    if(Devlink2::DEV_SUCCESS == m_proto_ptr->encodeMsg(msg_id, msg.data(), tmp))
    {
        emit sigPacked(QByteArray(reinterpret_cast<char*>(&tmp), sizeof(tmp)));
    }
}