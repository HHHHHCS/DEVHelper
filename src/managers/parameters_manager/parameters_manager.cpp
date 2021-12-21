#include "parameters_manager.h"


using namespace managers;

ParametersManager::ParametersManager(ManagerCollection* man_collect)
    : QObject(reinterpret_cast<QObject*>(man_collect))
    , m_p_manager_collect(man_collect)
{

}

ParametersManager::~ParametersManager()
{

}