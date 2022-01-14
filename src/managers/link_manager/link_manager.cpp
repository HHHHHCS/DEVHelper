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
    if(m_p_serial_scan_thread->joinable())
    {
        m_p_serial_scan_thread->join();
    }
    m_p_serial_scan_thread = nullptr;

    disconnectAll();

}

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
                    emit fetchLinkInfoMap(m_link_info_map);
                    m_link_info_map.clear();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    // TODO(huangchsh): 创建可连接TCP或UDP搜索线程

    // TODO(huangchsh): 创建其他可连接搜索线程
}

// TODO(huangchsh): 优化项，确定单个连接对应的任务队列个数
void LinkManager::createChoiceLink(const QString name)
{
    // TODO(huangchsh): 暂时只先实现串口类型，其余类型后续添加
    communication::SharedLinkPtr p_create_link = std::make_shared<communication::SerialLink>(name, 921600);

    // TODO(huangchsh): 通信错误信号
    // connect(p_create_link->get(), &communication::Link::commError, , );
    // TODO(huangchsh): 读取信号
    // connect(p_create_link->get(), &communication::Link::bytesReceived, , );
    // TODO(huangchsh): 写入信号
    // connect(p_create_link->get(), &communication::Link::bytesSent, , );
    connect(p_create_link.get(), &communication::Link::disconnected, this, &LinkManager::linkDisconnected);

    if(p_create_link->connect())
    {
        // 连接成功后，创建任务队列，插入map
        QList<tasks::SharedPtrTasksQueue> link_task_q_list;

        tasks::SharedPtrTasksQueue p_link_task_queue = std::make_shared<tasks::TasksQueue>(name + "_q_1");

        link_task_q_list.append(p_link_task_queue);

        qDebug("%s connected", p_create_link->getPortName().toStdString().data());

        emit addLink();

        m_links_map.insert(p_create_link, link_task_q_list);
    }
    else
    {
        qDebug("%s connect failed", p_create_link->getPortName().toStdString().data());
    }
}

void LinkManager::removeChoiceLink(const QString name)
{
    communication::SharedLinkPtr p_link = getSharedLinkPtrByLinkName(name);
    if(p_link.get())
    {
        p_link->disconnect();
        qDebug("%s disconnected", p_link->getPortName().toStdString().data());
    }
}

void LinkManager::disconnectAll()
{
    searchLinkToDo([](communication::SharedLinkPtr link_shptr)
    {
        link_shptr->disconnect();

        return link_shptr;
    });
}

void LinkManager::linkDisconnected()
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
    disconnect(link, &communication::Link::disconnected, this, &LinkManager::linkDisconnected);

    searchLinkToDo([&link, this](communication::SharedLinkPtr link_shptr)
    {
        if(link_shptr.get() == link)
        {
            m_links_map.remove(link_shptr);

            return link_shptr;
        }

        return communication::SharedLinkPtr(nullptr);
    });
}

communication::SharedLinkPtr LinkManager::getSharedLinkPtrByLinkName(const QString name)
{
    auto p_link = searchLinkToDo([name](communication::SharedLinkPtr link_shptr)
    {
        if(link_shptr.get()->getPortName() == name)
        {
            return link_shptr;
        }

        return communication::SharedLinkPtr(nullptr);
    });

    return p_link;
}

communication::SharedLinkPtr LinkManager::searchLinkToDo(LinkOptFunc func)
{
    communication::SharedLinkPtr link_ptr = nullptr;

    LinkTasksQueueMap& links_map = m_links_map;

    LinkTasksQueueMapConstIter iter = links_map.cbegin();
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