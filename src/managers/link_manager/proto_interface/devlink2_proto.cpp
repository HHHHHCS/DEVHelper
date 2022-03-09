#include <QDebug>

#include "devlink2_proto.h"


using namespace communication;

Devlink2Proto::Devlink2Proto(const SharedPtrLink& link, bool do_self_thread /* = true */)
    : Proto(ProtoType::DEVLINK2, ("dlink2-" + link->getPortName()))
    , m_proto_ptr(nullptr)
{
    if(do_self_thread)
    {
        m_proto_ptr = std::make_unique<devconn::Devlink2>();
    }
}

Devlink2Proto::~Devlink2Proto()
{
}

/**
 * @brief 协议解析槽
 * @note 在槽内完成协议解析并进行其他处理
 */
void Devlink2Proto::slotParseProto(const QByteArray&)
{

}

/**
 * @brief 协议封装槽
 * @note 在槽内完成协议封装并进行其他处理，最后转发至通信端口
 */
void Devlink2Proto::slotPackProto(const QByteArray&)
{

}