#include "link_manager.h"
#include "main_manager.h"
#include "parameters_manager.h"
#include "upgrade_manager.h"


using namespace managers;

ManagerCollection::ManagerCollection(QApplication *app)
    : QObject()
    , m_p_app(app)
    , m_p_link_man(nullptr)
    , m_p_main_man(nullptr)
    , m_p_param_man(nullptr)
    , m_p_upgrade_man(nullptr)
{
    m_p_main_man = new MainManager(app, this);

    m_p_link_man = new LinkManager(app, this);
    m_p_param_man = new ParametersManager(app, this);
    m_p_upgrade_man = new UpgradeManager(app, this);
}

ManagerCollection::~ManagerCollection()
{
    delete m_p_link_man;
    delete m_p_param_man;
    delete m_p_upgrade_man;
    delete m_p_main_man;
}

Manager::Manager(QApplication *app, ManagerCollection *man_collect)
    : QObject(man_collect)
    , m_app(app)
    , m_man_collect(man_collect)
{
}

Manager::~Manager()
{
    m_app = nullptr;
    m_man_collect = nullptr;
}