#include <memory>

#include <QList>
#include <QApplication>
#include <QDebug>
// #include <QSignalSpy>

#include "link_manager.h"
#include "dev_helper_application.h"


using namespace managers;

LinkManager::LinkManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{
    // TODO(huangchsh): 创建连接
}

LinkManager::~LinkManager()
{

}

int LinkManager::foundLinksList()
{
    // TODO(huangchsh): 扫描USB列表
    // TODO(huangchsh): 扫描网络列表
    return 0;
}