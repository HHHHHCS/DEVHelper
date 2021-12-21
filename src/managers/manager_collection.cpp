#include "manager_collection.h"


using namespace managers;

ManagerCollection::ManagerCollection(QApplication *app)
    : QObject()
    , m_p_app(app)
    , m_p_link_man(nullptr)
    , m_p_main_man(nullptr)
    , m_p_param_man(nullptr)
    , m_p_upgrade_man(nullptr)
{
    m_p_main_man = new MainManager(this);

    m_p_link_man = new LinkManager(this);
    m_p_param_man = new ParametersManager(this);
    m_p_upgrade_man = new UpgradeManager(this);
}

ManagerCollection::~ManagerCollection()
{
    delete m_p_link_man;
    delete m_p_param_man;
    delete m_p_upgrade_man;
    delete m_p_main_man;
}