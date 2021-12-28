#include <future>

#include <QApplication>
#include <QDebug>
// #include <QSignalSpy>

#include "link_manager.h"
// #include "dev_helper_application.h"


using namespace managers;

LinkManager::LinkManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{

}

LinkManager::~LinkManager()
{

}

void LinkManager::createScanLinksListWork()
{
    // TODO(huangchsh): 扫描USB列表
    // 搜索可连接端口
    std::async(std::launch::async, [this]()
    {
        bool is_continue = true;
        while(is_continue)
        {
            // TODO(huangchsh): 此处搜索可连接端口插入列表，发出列表信号
            QList<port_lib::PortInfoStru> ports_list;
            ports_list.clear();
            ports_list = communication::SerialLink::findPortsListForward();
            if(!ports_list.isEmpty())
            {
                for(auto &iter : ports_list)
                {
                    // TODO(huangchsh): 增加设备名列表内搜索
                    // if(iter.description.to_lower().contains(desc))
                    // {

                    // }
                }

                is_continue = false;
            }
        }
    });

    // TODO(huangchsh): 扫描网络列表
}