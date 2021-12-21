#include "link_manager.h"


using namespace managers;

LinkManager::LinkManager(ManagerCollection* man_collect)
    : QObject(nullptr)
    , m_p_manager_collect(man_collect)
{

}

LinkManager::~LinkManager()
{

}