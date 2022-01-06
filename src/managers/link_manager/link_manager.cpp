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
    // 1. 创建搜索可连接USB或串口端口的线程
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

                    m_link_info_map.insert(iter.name, iter.description);
                }

                // TODO(huangchsh): 需要对设备通过类型进行区分及显示
                if(!m_link_info_map.isEmpty())
                {
                    emit linkInfoMap(m_link_info_map);
                    m_link_info_map.clear();
                }
            }

            std::this_thread::sleep_for(std::chrono::seconds(3));
        }
    });

    // TODO(huangchsh): 创建搜索可连接USB或串口端口的线程
}

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

    m_links_list.append(p_create_link);

    p_create_link->connect();

    emit linkAdded();
}

void LinkManager::disconnectAll()
{
    QList<communication::SharedLinkPtr> lins_ptr_list = m_links_list;

    for(const auto &link : lins_ptr_list)
    {
        link->disconnect();
    }
}

void LinkManager::linkDisconnected()
{
    communication::Link* link = qobject_cast<communication::Link*>(sender());

    if(!link || !isBelong2LinksList(link))
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

    for(int i = 0; i < m_links_list.count(); ++i)
    {
        if(m_links_list[i].get() == link)
        {
            m_links_list.removeAt(i);

            return;
        }
    }
}

communication::SharedLinkPtr LinkManager::getSharedLinkPtrByLinkName(const QString name)
{
    for (int i = 0; i< m_links_list.count(); i++)
    {
        communication::SharedLinkPtr link = m_links_list[i];
        if (link.get()->getPortName() == name)
        {
            return link;
        }
    }

    return communication::SharedLinkPtr(nullptr);
}

bool LinkManager::isBelong2LinksList(communication::Link* link)
{
    for(int i = 0; i < m_links_list.count(); ++i)
    {
        if(m_links_list[i].get() == link)
        {
            return true;
        }
    }

    return false;
}