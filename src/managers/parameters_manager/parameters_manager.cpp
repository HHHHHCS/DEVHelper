﻿#include <QDebug>

#include "link_manager.h"
#include "parameters_manager.h"


using namespace managers;

ParametersManager::ParametersManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
    , m_dev_params_map()
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

        // 1. 发送获取请求
        if(p_proto->getProtoType() == communication::Proto::ProtoType::DEVLINK2)
        {
            awlink_query_or_modify_dev_params_t msg{0};
            // TODO(huangchsh): 填充消息内容
            p_proto->sigPack(AWLINK_MSG_ID_QUERY_OR_MODIFY_DEV_PARAMS, QByteArray(reinterpret_cast<char*>(&msg), sizeof(msg)));
        }
        else
        {
            throw 2;
        }
        // 2. 等待反馈
        // TODO(huangchsh): 设置超时时间
        if(p_proto->getProtoType() == communication::Proto::ProtoType::DEVLINK2)
        {
            auto result(false);
            // 4. 发出参数列表信号
            if(result)
            {
                emit sigFetchParametersMap(link_name, m_dev_params_map[link_name]);
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
 * @brief 增加参数
 * @note 供QML调用
 * @param[in] link_name: 连接名
 * @param[in] link_params_map: 若干参数（包括参数名及参数值）
 * @return 参数名对应的参数值
 *          -1: 查找失败或未查找到
 */
void ParametersManager::addParameters(const QString link_name, const ParametersMap link_params_map)
{
    if(!m_dev_params_map.contains(link_name))
    {
        ParametersMapConstIterator citer = link_params_map.cbegin();
        while(citer != link_params_map.cend())
        {
            if(!m_dev_params_map[link_name].contains(citer.key()))
            {
                m_dev_params_map[link_name].insert(citer.key(), citer.value());

                qDebug() << link_name << " add parameter name: " << citer.key() << " value: " << citer.value() << endl;
            }

            ++citer;
        }
    }
    else
    {
        m_dev_params_map.insert(link_name, link_params_map);

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
    if(m_dev_params_map.contains(link_name)
    && m_dev_params_map[link_name].contains(param_name))
    {
        m_dev_params_map[link_name].remove(param_name);

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
    QVariantList params_name_list;

    if(m_dev_params_map.contains(link_name))
    {
        ParametersMapConstIterator citer = link_params_map.cbegin();
        while(citer != link_params_map.cend())
        {
            if(m_dev_params_map[link_name].contains(citer.key()))
            {
                qDebug() << link_name << " modify parameter name: " << citer.key() << " from value: " << m_dev_params_map[link_name][citer.key()] << " to value: " << citer.value() << endl;

                m_dev_params_map[link_name][citer.key()] = citer.value();

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
    if(m_dev_params_map.contains(link_name))
    {
        qDebug() << "Not found link " << link_name << endl;
        return -1;
    }

    if(!m_dev_params_map[link_name].contains(param_name))
    {
        qDebug() << "Not found link " << param_name << endl;
        return -2;
    }

    qDebug() << "Found parameter name: " << link_name << " value: " << param_name << endl;

    return m_dev_params_map[link_name][param_name];
}