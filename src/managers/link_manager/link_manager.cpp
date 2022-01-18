#include <memory>
#include <chrono>

#include <QApplication>
#include <QDebug>
// #include <QSignalSpy>

#include "link_manager.h"


using namespace managers;

LinkManager::LinkManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
    , m_serial_scan_thread_stop_flag(false)
{
}

LinkManager::~LinkManager()
{
    m_serial_scan_thread_stop_flag = true;
    // 回收检测端口线程资源
    if(m_p_serial_scan_thread)
    {
        if(m_p_serial_scan_thread->joinable())
        {
            m_p_serial_scan_thread->join();
        }
        m_p_serial_scan_thread = nullptr;
    }

    disconnectAll();

}

/**
 * @brief 创建可用连接列表扫描线程
 * @note 扫描周期3s
 */
void LinkManager::createScanLinksListWork()
{
    // 1. 创建可连接USB或串口端口的搜索线程
    m_p_serial_scan_thread = new std::thread([this]()
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
                    // for(auto link_dev_name : LINK_DEV_NAME_LIST)
                    // {
                    //     if(iter.description.toLower().contains(iter.name))
                    //     {
                    //         m_port_name_list.append(iter.name);
                    //     }
                    // }

                    // NOTE(huangchsh): 调试代码
                    m_link_info_map.insert(iter.name, iter.description);
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

    // TODO(huangchsh): 创建可连接TCP或UDP搜索线程

    // TODO(huangchsh): 创建其他可连接搜索线程
}

/**
 * @brief 创建用户于UI的所选连接
 * @note 建立连接后，创建任务队列，并在UI增加连接选项卡
 * @todo(huangchsh): 优化项，确定单个连接对应的任务队列个数
 * @param[in] name: 端口名，如COM1
 */
void LinkManager::createChoiceLink(const QString name)
{
    // TODO(huangchsh): 同名连接检测
    // TODO(huangchsh): 暂时只先实现串口类型，其余类型后续添加
    communication::SharedPtrLink p_create_link = std::make_shared<communication::SerialLink>(name, 921600);
    if(!p_create_link)
    {
        return;
    }

    // TODO(huangchsh): 通信错误信号
    // connect(p_create_link->get(), &communication::Link::commError, , );
    // TODO(huangchsh): 读取信号
    // connect(p_create_link->get(), &communication::Link::bytesReceived, , );
    // TODO(huangchsh): 写入信号
    // connect(p_create_link->get(), &communication::Link::bytesSent, , );
    connect(p_create_link.get(), &communication::Link::disconnected, this, &LinkManager::slotsLinkDisconnected);

    if(p_create_link->connect())
    {
        // 连接成功后，创建任务队列，插入map
        // TODO(huangchsh): 通过index支持多条队列
        QList<tasks::SharedPtrTasksQueue> link_task_q_list;
        tasks::SharedPtrTasksQueue p_link_task_queue = std::make_shared<tasks::TasksQueue>(name.toStdString() + "_q_1");
        link_task_q_list.append(p_link_task_queue);

        qDebug("%s connected", p_create_link->getPortName().toStdString().data(), link_task_q_list.count());

        m_links_map.insert(p_create_link, link_task_q_list);

        emit sigAddLink();
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
    communication::SharedPtrLink p_link = getSharedPtrLinkByName(name);
    if(p_link
    && p_link.get())
    {
        p_link->disconnect();
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
 * @brief 连接断开槽
 * @note 对应信号 communication::Link::disconnected
 */
void LinkManager::slotsLinkDisconnected()
{
    communication::Link* link = qobject_cast<communication::Link*>(sender());
    if(!link)
    {
        return;
    }

    // TODO(huangchsh): 断连通信错误信号
    // disconnect(link, &communication::Link::commError, , );
    // TODO(huangchsh): 断连读取信号
    // disconnect(link, &communication::Link::bytesReceived, , );
    // TODO(huangchsh): 断连写入信号
    // disconnect(link, &communication::Link::bytesSent, , );
    disconnect(link, &communication::Link::disconnected, this, &LinkManager::slotsLinkDisconnected);

    searchLinkToDo([&link, this](communication::SharedPtrLink link_shptr)
    {
        if(link_shptr
        && link_shptr.get() == link)
        {
            m_links_map.remove(link_shptr);

            return link_shptr;
        }

        return communication::SharedPtrLink(nullptr);
    });
}

communication::SharedPtrLink LinkManager::getSharedPtrLinkByName(const QString name)
{
    return searchLinkToDo([name](communication::SharedPtrLink link_shptr)
    {
        if(link_shptr
        && link_shptr.get()->getPortName() == name)
        {
            return link_shptr;
        }

        return communication::SharedPtrLink(nullptr);
    });
}

tasks::SharedPtrTasksQueue LinkManager::getFreeSharedPtrTasksQueueByLink(const QString name)
{
    communication::SharedPtrLink p_link = getSharedPtrLinkByName(name);
    if(!p_link)
    {
        return tasks::SharedPtrTasksQueue(nullptr);
    }

    // TODO(huangchsh): 筛选当前空闲队列
    TasksQueuePtrList tasks_q_ptr_list = m_links_map[p_link];
    for(auto &item : tasks_q_ptr_list)
    {
        // TODO(huangchsh): 通过index支持多条队列
        if(item->getName() == (name.toStdString() + "_q_1"))
        {
            return item;
        }
    }
    return tasks::SharedPtrTasksQueue(nullptr);
}

communication::SharedPtrLink LinkManager::searchLinkToDo(LinkOptFunc func)
{
    communication::SharedPtrLink link_ptr = nullptr;

    LinkTasksQueueMap& links_map = m_links_map;

    LinkTasksQueueMapConstIterator iter = links_map.cbegin();
    while(iter != links_map.cend())
    {
        auto iter_link = iter.key();

        link_ptr = func(iter_link);
        if(link_ptr)
        {
            break;
        }

        ++iter;
    }

    return link_ptr;
}