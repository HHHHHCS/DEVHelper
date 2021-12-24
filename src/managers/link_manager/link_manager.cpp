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

}

LinkManager::~LinkManager()
{

}

void loadLinkConfigurationList()
{
    
}