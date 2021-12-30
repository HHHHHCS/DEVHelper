#include <thread>
#include <memory>
#include <future>

#include <QApplication>
#include <QDebug>
// #include <QSignalSpy>

#include "link_manager.h"


using namespace managers;

LinkManager::LinkManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{
    // connect(this, )
}

LinkManager::~LinkManager()
{
    disconnectAll();
}

void LinkManager::createScanLinksListWork()
{
    // 1. 创建搜索可连接USB或串口端口的线程
    std::thread find_links_list_thread([this]()
    {
        QList<port_lib::PortInfoStru> ports_list;
        while(1)
        {
            ports_list.clear();
            ports_list = communication::SerialLink::findPortsListForward();
            if(ports_list.isEmpty())
            {
                for(auto &iter : ports_list)
                {
                    for(auto link_dev_name : LINK_DEV_NAME_LIST)
                    {
                        if(iter.description.toLower().contains(link_dev_name))
                        {
                            m_port_name_list.append(link_dev_name);
                        }
                    }
                }

                // TODO(huangchsh): 发出列表信号
            }
        }
    });
    find_links_list_thread.detach();

    // TODO(huangchsh): 扫描网络端口列表
}

void LinkManager::createChoiceLink(const QString name)
{
    communication::SharedLinkPtr p_create_link = std::make_shared<communication::SerialLink>(name, 921600);

    p_create_link->connect();

    m_links_list.append(p_create_link);
}

void LinkManager::disconnectAll()
{
    QList<communication::SharedLinkPtr> lins_ptr_list = m_links_list;

    for(const auto &link : lins_ptr_list)
    {
        // TODO(huangchsh): 信号与槽断连

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

    // TODO(huangchsh): 信号与槽断连

    for(int i = 0; i < m_links_list.count(); ++i)
    {
        if(m_links_list[i].get() == link)
        {
            m_links_list.removeAt(i);

            return;
        }
    }
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