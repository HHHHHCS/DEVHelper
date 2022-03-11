#include "link_manager.h"
#include "main_manager.h"
#include "parameters_manager.h"
#include "upgrade_manager.h"


using namespace managers;

ManagerCollection::ManagerCollection(QApplication *app)
    : QObject()
    , m_p_main_man(std::make_shared<MainManager>(app, this))
    , m_p_link_man(std::make_shared<LinkManager>(app, this))
    , m_p_param_man(std::make_shared<ParametersManager>(app, this))
    , m_p_upgrade_man(std::make_shared<UpgradeManager>(app, this))
{
}

Manager::Manager(QApplication *app, ManagerCollection *man_collect)
    : QObject(man_collect)
    , m_app(app)
    , m_man_collect(man_collect)
{
}