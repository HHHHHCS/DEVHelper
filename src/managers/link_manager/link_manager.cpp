#include "link_manager.h"


using namespace managers;

LinkManager::LinkManager(ManagerCollection* man_collect)
    : QObject(reinterpret_cast<QObject*>(man_collect))
    , m_p_manager_collect(man_collect)
{

}

LinkManager::~LinkManager()
{

}