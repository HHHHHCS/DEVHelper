#include "upgrade_manager.h"


using namespace managers;

UpgradeManager::UpgradeManager(ManagerCollection* man_collect)
    : QObject(reinterpret_cast<QObject*>(man_collect))
    , m_p_manager_collect(man_collect)
{

}

UpgradeManager::~UpgradeManager()
{

}