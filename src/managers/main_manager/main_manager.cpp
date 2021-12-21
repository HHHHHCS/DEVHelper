#include "main_manager.h"


using namespace managers;

MainManager::MainManager(ManagerCollection* man_collect)
    : QObject(nullptr)
    , m_p_manager_collect(man_collect)
{

}

MainManager::~MainManager()
{

}