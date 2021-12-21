#include "parameters_manager.h"


using namespace managers;

ParametersManager::ParametersManager(ManagerCollection* man_collect)
    : QObject(nullptr)
    , m_p_manager_collect(man_collect)
{

}

ParametersManager::~ParametersManager()
{

}