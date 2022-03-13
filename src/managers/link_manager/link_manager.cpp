#include <memory>
#include <chrono>

#include <QApplication>
#include <QDebug>
// #include <QSignalSpy>

#include "link_manager.h"


using namespace managers;

LinkManager::LinkManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
    , m_p_serial_scan_thread_ptr(nullptr)
    , m_serial_scan_thread_stop_flag(false)
    , m_link_info_map{}
    , m_link_block_map{}
{
}

LinkManager::~LinkManager()
{
    m_serial_scan_thread_stop_flag = true;
    // 回收检测端口线程资源
    if(m_p_serial_scan_thread_ptr)
    {
        if(m_p_serial_scan_thread_ptr->joinable())
        {
            m_p_serial_scan_thread_ptr->join();
        }
        while(m_p_serial_scan_thread_ptr->joinable());
    }

    m_link_block_map.clear();
}

/**
 * @brief 创建可用连接列表扫描线程
 * @note 扫描周期3s
 */
void LinkManager::createScanLinksListWork()
{
    // 1. 创建可连接USB或串口端口的搜索线程
    m_p_serial_scan_thread_ptr = std::make_unique<std::thread>([this]()
    {
        QList<port_lib::PortInfoStru> links_list;
        while(!m_serial_scan_thread_stop_flag)
        {
            links_list.clear();
            links_list = communication::SerialLink::findPortsListForward();
            if(!links_list.isEmpty())
            {
                for(auto &iter : links_list)
                {
                    for(auto &link_dev_name : LINK_DEV_NAME_LIST)
                    {
                        // TODO(huangchsh): 由于驱动环境原因，此处用序列号进行识别，后期进行优化
                        if(iter.serial_number.toLower().contains(link_dev_name))
                        {
                            m_link_info_map.insert(iter.name, iter.description);
                        }
                    }

                    // NOTE(huangchsh): 调试代码
                    // m_link_info_map.insert(iter.name, iter.description);
                }

                // TODO(huangchsh): 需要对设备通过类型进行区分及显示
                if(!m_link_info_map.isEmpty())
                {
                    emit sigFetchLinkInfoMap(m_link_info_map);
                    m_link_info_map.clear();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    // TODO(huangchsh): 2.创建可连接TCP或UDP搜索线程

    // TODO(huangchsh): 3.创建其他可连接搜索线程
}

/**
 * @brief 创建用户于UI的所选连接
 * @note 建立连接后，创建任务队列，并在UI增加连接选项卡
 * @todo(huangchsh): 优化项，确定单个连接对应的任务队列个数
 * @param[in] name: 端口名，如COM1
 */
void LinkManager::createChoiceLink(const QString name)
{
    if(name.isEmpty())
    {
        return;
    }

    // 同名连接检测
    if(getSharedPtrLinkByName(name) != nullptr)
    {
        return;
    }

    communication::SharedPtrLink p_create_link;

    // USB-串口
    if(name.contains("COM"))
    {
        p_create_link = std::make_shared<communication::SerialLink>(name, 115200);
        if(!p_create_link)
        {
            return;
        }
    }
    else
    {
        ;
    }

    if(p_create_link->connect())
    {
        // 连接成功后，创建任务队列，插入map
        LinkBlock link_block;

        // TODO(huangchsh): 此逻辑可扩展，不应将协议应用局限于连接类型
        const auto& link_type = p_create_link->getLinkType();
        switch (link_type)
        {
            case communication::Link::LinkType::SERIAL:
            {
                link_block.p_proto = std::make_unique<communication::Devlink2Proto>(name, static_cast<awlink_channel_t>(m_link_block_map[p_create_link].tasks_q_list.size()));
                break;
            }
            case communication::Link::LinkType::CAN:
            {
                break;
            }
            case communication::Link::LinkType::TCP:
            {
                break;
            }
            case communication::Link::LinkType::UDP:
            {
                break;
            }
            case communication::Link::LinkType::UNKNOWN_TYPE:
            default:
            {
                break;
            }
        }

        if(!link_block.p_proto)
        {
            return;
        }

        // TODO(huangchsh): 通过index支持多条队列
        auto& p_link_task_queue = std::make_shared<tasks::TasksQueue>(name.toStdString() + "_q_" + std::to_string(m_link_block_map[p_create_link].tasks_q_list.size()));
        link_block.tasks_q_list.append(p_link_task_queue);
        m_link_block_map.insert(p_create_link, link_block);

        connect(p_create_link.get(), &communication::Link::sigCommError, this, &LinkManager::slotLinkCommError);
        connect(p_create_link.get(), &communication::Link::sigDisconnected, this, &LinkManager::slotLinkDisconnected);
        connect(p_create_link.get(), &communication::Link::sigBytesReceived, link_block.p_proto.get(), &communication::Proto::slotParseProto);
        connect(link_block.p_proto.get(), &communication::Proto::sigPack, link_block.p_proto.get(), &communication::Proto::slotPackProto);
        connect(link_block.p_proto.get(), &communication::Proto::sigPacked, p_create_link.get(), &communication::Link::slotWriteBytes);

        // 添加默认任务：发送心跳
        tasks::Task task(p_link_task_queue->getName() + "heartbeat", p_link_task_queue->getName() , [p_proto = link_block.p_proto, this]()
        {
            static uint8_t msg_seq;

            if(p_proto->getProtoType() == communication::Proto::ProtoType::DEVLINK2)
            {
                awlink_host_heartbeat_t msg
                {
                    // .time_boot_ms = 0,
                    // // .connection_num = m_link_block_map.count(),
                    // .seq = msg_seq++,
                    // // .type = RC,
                    // // .id = 0,
                    // // .task_queue_num,
                    // // .state,
                    // .sw_ver = 1,
                    // .proto_ver = 2,
                };

                p_proto->sigPack(AWLINK_MSG_ID_HOST_HEARTBEAT, QByteArray(reinterpret_cast<char*>(&msg), sizeof(awlink_host_heartbeat_t)));
            }

            // TODO(huangchsh): 更新连接及选项卡状态
            // emit sigUpdateLinkStatus(status);
        });
        bool result = p_link_task_queue->addFastTask(task);
        if(result)
        {
            qDebug() << "Append task" << task.getName().data() << "into queue" << p_link_task_queue->getName().data() << endl;
        }
        else
        {
            qDebug() << "Failed append task" << task.getName().data() << "into queue" << p_link_task_queue->getName().data() << endl;
        }

        emit sigAddLink();

        qDebug("%s connected", p_create_link->getPortName().toStdString().data(), link_block.tasks_q_list.count());
    }
    else
    {
        qDebug("%s connect failed", p_create_link->getPortName().toStdString().data());
    }
}

/**
 * @brief 删除用户于UI的所选连接
 * @note UI中删除连接选项卡后，删除连接
 * @param[in] name: UI选项卡对应连接名，端口名，如COM1
 */
void LinkManager::removeChoiceLink(const QString name)
{
    auto& p_link = getSharedPtrLinkByName(name);
    if(p_link)
    {
        p_link->disconnect();
        m_link_block_map.remove(p_link);
        qDebug("%s disconnected", p_link->getPortName().toStdString().data());
    }
}

/**
 * @brief 断开连接列表中所有连接
 */
void LinkManager::disconnectAll()
{
    searchLinkToDo([](communication::SharedPtrLink link_shptr)
    {
        if(link_shptr)
        {
            link_shptr->disconnect();
            return link_shptr;
        }

        return communication::SharedPtrLink(nullptr);
    });
}

/**
 * @brief 通信错误槽
 * @note 对应信号 communication::Link::sigCommError
 * @param[in] link_name 出现异常的连接名
 * @param[in] link_error 出现的具体异常
 */
void LinkManager::slotLinkCommError(const QString& link_name, const QString& link_error)
{
    // TODO(huangchsh): 后续进行处理
}

/**
 * @brief 连接断开槽
 * @note 对应信号 communication::Link::sigDisconnected
 */
void LinkManager::slotLinkDisconnected()
{
    const auto& link = qobject_cast<communication::Link*>(sender());
    if(!link)
    {
        return;
    }

    const auto& link_proto = getSharedPtrProtoByLinkName(communication::SharedPtrLink(link)->getPortName());
    if(link_proto)
    {
        disconnect(link, &communication::Link::sigBytesReceived, link_proto.get(), &communication::Proto::slotParseProto);
    }
    disconnect(link, &communication::Link::sigCommError, this, &LinkManager::slotLinkCommError);
    disconnect(link, &communication::Link::sigDisconnected, this, &LinkManager::slotLinkDisconnected);

    searchLinkToDo([&link, this](communication::SharedPtrLink link_shptr)
    {
        if(link_shptr
        && link_shptr.get() == link)
        {
            m_link_block_map.remove(link_shptr);

            return link_shptr;
        }

        return communication::SharedPtrLink(nullptr);
    });
}

communication::SharedPtrLink LinkManager::getSharedPtrLinkByName(const QString& name)
{
    return searchLinkToDo([name](communication::SharedPtrLink link_shptr)
    {
        if(link_shptr
        && link_shptr->getPortName() == name)
        {
            return link_shptr;
        }

        return communication::SharedPtrLink(nullptr);
    });
}

communication::SharedPtrProto LinkManager::getSharedPtrProtoByLinkName(const QString& name)
{
    const auto& link = getSharedPtrLinkByName(name);
    if(m_link_block_map.contains(link))
    {
        if(m_link_block_map[link].p_proto)
        {
            return m_link_block_map[link].p_proto;
        }
    }

    return communication::SharedPtrProto(nullptr);
}

tasks::SharedPtrTasksQueue LinkManager::getFreeSharedPtrTasksQueueByLink(const QString& name)
{
    auto& p_link = getSharedPtrLinkByName(name);
    if(!p_link)
    {
        return tasks::SharedPtrTasksQueue(nullptr);
    }

    // TODO(huangchsh): 筛选当前空闲队列
    for(auto& item : m_link_block_map[p_link].tasks_q_list)
    {
        // TODO(huangchsh): 通过index支持多条队列
        if(item->getName() == (name.toStdString() + "_q_0"))
        {
            return item;
        }
    }
    return tasks::SharedPtrTasksQueue(nullptr);
}

communication::SharedPtrLink LinkManager::searchLinkToDo(std::function<communication::SharedPtrLink(const communication::SharedPtrLink&)> func)
{
    communication::SharedPtrLink link_ptr = nullptr;

    auto& list = m_link_block_map;

    auto& iter = list.cbegin();
    while(iter != list.cend())
    {
        link_ptr = func(iter.key());
        if(link_ptr)
        {
            break;
        }

        ++iter;
    }

    return link_ptr;
}