#include "main_manager.h"
#include "link_manager.h"

#include "dev_helper_application.h"


using namespace dev_helper;
using namespace app;

DevHelperApplication* DevHelperApplication::m_s_app_inst = nullptr;

DevHelperApplication::DevHelperApplication(int &argc, char* argv[])
    : QApplication(argc, argv)
    , m_p_qml_app_engine(nullptr)
    , m_p_man_collect(nullptr)
{
    m_s_app_inst = this;

    // TODO(huangchsh): 创建QTimer记录运行时长及启动时间

    m_p_man_collect = new managers::ManagerCollection(m_s_app_inst);
    Q_CHECK_PTR(m_p_man_collect);
}

DevHelperApplication::~DevHelperApplication()
{
    if(m_p_qml_app_engine)
    {
        delete m_p_qml_app_engine;
    }

    if(m_p_man_collect)
    {
        delete m_p_man_collect;
    }

    m_p_qml_app_engine = nullptr;
    m_p_man_collect = nullptr;
}

void DevHelperApplication::shutdown()
{
    delete m_p_qml_app_engine;
    delete m_p_man_collect;
}

bool DevHelperApplication::bootMainWindow()
{
    m_p_qml_app_engine = man_collection()->main_man()->createQMLAppEngine(this);
    man_collection()->main_man()->createMainWindow(m_p_qml_app_engine);

    if(m_p_qml_app_engine->rootObjects().isEmpty())
    {
        return false;
    }

    man_collection()->link_man()->foundLinksList();

    return true;
}

DevHelperApplication* devHelperApp(void)
{
    return DevHelperApplication::m_s_app_inst;
}
