#include <QDebug>

#include "link_manager.h"
#include "parameters_manager.h"


using namespace managers;

ParametersManager::ParametersManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
    , m_dev_params_map()
    , m_dev_param_fetch_list_msg_seq(0)
    , m_is_ever_fetched_params_list(false)
    , m_is_waitting_for_params_list(false)
{
    connect(this, &ParametersManager::sigRequestParametersMap, this, &ParametersManager::slotsCreateParametersMap);
}

ParametersManager::~ParametersManager()
{
    disconnect(this, &ParametersManager::sigRequestParametersMap, this, &ParametersManager::slotsCreateParametersMap);
}

/**
 * @brief 获取及创建参数列表
 * @note 对应信号 sigRequestParametersMap
 * @param[in] link_name: 请求参数列表信号中的连接名
 */
void ParametersManager::slotsCreateParametersMap(const QString link_name)
{
    auto& task_queue(man_collect()->link_man()->getFreeSharedPtrTasksQueueByLink(link_name));
    if(!task_queue)
    {
        qDebug() << "Failed to request " << link_name.toStdString().data() << " parameters map" << endl;
        return;
    }

    tasks::Task task("get_param_list", task_queue->getName(), 0, [&, this]()
    {
        auto& p_proto(man_collect()->link_man()->getSharedPtrProtoByLinkName(link_name));
        if(!p_proto)
        {
            qDebug() << "Failed to get pointer of link " << link_name.toStdString().data() << " proto " << p_proto->getProtoName() << endl;
            throw 1;
        }

        connect(p_proto.get(), &communication::Proto::sigParsed, this, &ParametersManager::slotsRespParametersOperation);
        // 1. 发送获取请求
        if(communication::Proto::ProtoType::DEVLINK2 == p_proto->getProtoType())
        {
            awlink_host_dev_param_fetch_list_t msg
            {
                ++m_dev_param_fetch_list_msg_seq,
            };

            uint32_t msg_id(AWLINK_MSG_ID_HOST_DEV_PARAM_FETCH_LIST);
            auto bytes(QByteArray(reinterpret_cast<char*>(&msg_id), sizeof(msg_id)));
            bytes.append(reinterpret_cast<char*>(&msg), sizeof(awlink_host_dev_param_fetch_list_t));
            p_proto->sigPack(bytes);
        }
        else
        {
            throw 2;
        }
        // 2. 等待反馈
        m_is_waitting_for_params_list = true;
        while(m_is_waitting_for_params_list);
        // TODO(huangchsh): 设置超时时间，等待参数获取完全
        if(communication::Proto::ProtoType::DEVLINK2 == p_proto->getProtoType())
        {
            auto result(false);
            // 4. 发出参数列表信号
            if(result)
            {
                {
                    QMutexLocker lock(&m_mutex_params_list_operation);
                    emit sigFetchParametersMap(link_name, m_dev_params_map[link_name].map);
                }
                m_is_ever_fetched_params_list = true;
            }
            printf("%s\r\n", "get_param_list");
        }
        else
        {
            throw 3;
        }
    });

    bool result = task_queue->addTask(task);
    if(result)
    {
        qDebug() << "Append task" << task.getName().data() << "into queue" << task_queue->getName().data() << endl;
    }
    else
    {
        qDebug() << "Failed append task" << task.getName().data() << "into queue" << task_queue->getName().data() << endl;
    }
}

/**
 * @brief 设备参数操作响应槽
 * @note 操作包括
 *              1. 查询
 *              2. 修改
 *              3. 获取列表
 * @note 对应信号 sigParsed
 */
void ParametersManager::slotsRespParametersOperation(const QByteArray& bytes)
{
    uint32_t msg_id(0);
    memcpy(&msg_id, bytes.data(), sizeof(msg_id));

    if(AWLINK_MSG_ID_DEV_PARAM_HOST_QUERY_OR_MODIFY_ACK == msg_id)
    {
        awlink_dev_param_host_query_or_modify_ack_t ack;
        memcpy(&ack, bytes.data() + 4, sizeof(awlink_dev_param_host_query_or_modify_ack_t));

        if(ack.fail_reason)
        {
            qDebug() << "Failed to do parameters " << (ack.action == PARAMETERS_OPERATION_FETCH_LIST ? "list fetch" :
                (ack.action == PARAMETERS_OPERATION_MODIFY ? "modify" :
                    (ack.action == PARAMETERS_OPERATION_QUERY ? "query" : "operation"))) << endl;
            return;
        }

        QString link_name;
        QString dev_type(ack.type == RC ? "RC" :
            (ack.type == RN ? "RN" :
                (ack.type == RP ? "RP" :
                    (ack.type == BA ? "BA" : "UNKNOWN"))));

        QMutexLocker lock(&m_mutex_params_list_operation);
        auto link_name_list(m_dev_params_map.keys());
        for(auto& list_item : link_name_list)
        {
            if(list_item.contains(dev_type))
            {
                auto id_str("-" + QString::number(ack.id));
                if(list_item.contains(id_str))
                {
                    link_name = list_item;
                    break;
                }
            }
        }

        if(link_name.isEmpty())
        {
            qDebug() << "Not found target link name" << endl;
            return;
        }

        if(m_dev_params_map.contains(link_name))
        {
            switch(ack.action)
            {
                case PARAMETERS_OPERATION_QUERY:
                {
                    auto param_id(QString(ack.param_id));
                    if(!m_dev_params_map[link_name].map.contains(param_id))
                    {
                        qDebug() << "Unknown parameters string ID" << param_id << endl;
                        break;
                    }

                    m_dev_params_map[link_name].map[param_id] = ack.param_value;
                    break;
                }
                case PARAMETERS_OPERATION_MODIFY:
                {
                    auto param_id(QString(ack.param_id));
                    if(!m_dev_params_map[link_name].map.contains(param_id))
                    {
                        qDebug() << "Unknown parameters string ID" << param_id << endl;
                        break;
                    }

                    // TODO(huangchsh): 发送修改完成信号
                    // m_dev_params_map[link_name].map[param_id] = ack.param_value;
                    break;
                }
                case PARAMETERS_OPERATION_FETCH_LIST:
                {
                    auto param_id(QString(ack.param_id));
                    auto params_list(m_dev_params_map[link_name].map);
                    if(!params_list.contains(param_id))
                    {
                        params_list.insert(QString(ack.param_id), ack.param_value);
                    }
                    else
                    {
                        params_list[param_id] = ack.param_value;
                    }

                    break;
                }
                default:
                {
                    qDebug() << "Unknown parameters operation" << endl;
                    return;
                }
            }
        }
    }
}

/**
 * @brief 增加参数
 * @note 供QML调用
 * @param[in] link_name: 连接名
 * @param[in] link_params_map: 若干参数（包括参数名及参数值）
 * @return 参数名对应的参数值
 *          -1: 查找失败或未查找到
 */
void ParametersManager::addParameters(const QString link_name, const ParametersMap link_params_map)
{
    QMutexLocker lock(&m_mutex_params_list_operation);

    if(!m_dev_params_map.contains(link_name))
    {
        auto params_list(m_dev_params_map[link_name].map);
        ParametersMapConstIterator citer = link_params_map.cbegin();
        while(citer != link_params_map.cend())
        {
            if(!params_list.contains(citer.key()))
            {
                params_list.insert(citer.key(), citer.value());

                qDebug() << link_name << " add parameter name: " << citer.key() << " value: " << citer.value() << endl;
            }

            ++citer;
        }
    }
    else
    {
        m_dev_params_map[link_name].map.insert(link_name, link_params_map);

        qDebug() << link_name << " add parameters" << endl;
    }
}

/**
 * @brief 删除参数
 * @note 供QML调用
 * @param[in] link_name: 连接名
 * @param[in] param_name: 参数名
 */
void ParametersManager::removeParameters(const QString link_name, const QString param_name)
{
    QMutexLocker lock(&m_mutex_params_list_operation);

    if(m_dev_params_map.contains(link_name))
    {
        auto params_list(m_dev_params_map[link_name].map);
        if(params_list.contains(param_name))
        {
            params_list.remove(param_name);
        }

        qDebug() << link_name << " remove parameter name: " << param_name << endl;
    }
}

/**
 * @brief 修改参数
 * @note 供QML调用，修改成功后发送修改参数成功信号，UI同步
 * @param[in] link_name: 连接名
 * @param[in] link_params_map: 若干参数（包括参数名及参数值）
 */
void ParametersManager::modifyParameters(const QString link_name, const ParametersMap link_params_map)
{
    QMutexLocker lock(&m_mutex_params_list_operation);

    QVariantList params_name_list;

    if(m_dev_params_map.contains(link_name))
    {
        auto params_list(m_dev_params_map[link_name].map);
        ParametersMapConstIterator citer = link_params_map.cbegin();
        while(citer != link_params_map.cend())
        {
            if(params_list.contains(citer.key()))
            {
                qDebug() << link_name << " modify parameter name: " << citer.key() << " from value: " << params_list[citer.key()] << " to value: " << citer.value() << endl;

                params_list[citer.key()] = citer.value();

                params_name_list.append(citer.key());
            }

            ++citer;
        }
    }

    emit sigModifyParametersSuccess(link_name, params_name_list);
}

/**
 * @brief 查找单个参数
 * @note 供QML调用
 * @param[in] link_name: 连接名
 * @param[in] param_name: 参数名
 * @return 参数名对应的参数值
 *          -1: 未查找到连接对应参数列表
 *          -2: 未查找到此参数
 */
QVariant ParametersManager::findParameters(const QString link_name, const QString param_name)
{
    QMutexLocker lock(&m_mutex_params_list_operation);

    if(m_dev_params_map.contains(link_name))
    {
        qDebug() << "Not found link " << link_name << endl;
        return -1;
    }

    auto params_list(m_dev_params_map[link_name].map);
    if(!params_list.contains(param_name))
    {
        qDebug() << "Not found parameter " << param_name << endl;
        return -2;
    }

    qDebug() << "Found parameter name: " << link_name << " value: " << param_name << endl;

    return params_list[param_name];
}

// TODO(huangchsh): XML参数文件输出