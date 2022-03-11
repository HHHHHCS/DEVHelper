#include "main_manager.h"
#include "link_manager.h"

#include "dev_helper_application.h"


using namespace dev_helper;
using namespace app;

DevHelperApplication::DevHelperApplication(int &argc, char* argv[])
    : QApplication(argc, argv)
    , m_p_qml_app_engine(nullptr)
    , m_p_man_collect(std::make_shared<managers::ManagerCollection>(this))
{
    // TODO(huangchsh): 创建QTimer记录运行时长及启动时间
}

void DevHelperApplication::shutdown()
{
    if(m_p_qml_app_engine)
    {
        m_p_qml_app_engine.reset();
    }

    if(m_p_man_collect)
    {
        m_p_man_collect.reset();
    }
}

void DevHelperApplication::initCommonType()
{
    // TODO(huangchsh): 后续类在此定义
    static const char* s_ref_only = "Reference only";
    static const char* s_devhelper = "DevHelper";
    static const char* s_dh_managers = "DevHelper.Managers";

    qmlRegisterUncreatableType<managers::LinkManager>(s_dh_managers, 1, 0, "LinkManager", s_ref_only);
}

bool DevHelperApplication::bootMainWindow()
{
    m_p_qml_app_engine = man_collection()->main_man()->createQMLAppEngine(this);
    man_collection()->main_man()->createMainWindow(m_p_qml_app_engine.get());

    if(m_p_qml_app_engine->rootObjects().isEmpty())
    {
        return false;
    }

    // 创建可用端口检测线程
    man_collection()->link_man()->createScanLinksListWork();

    return true;
}

QObject* DevHelperApplication::rootQmlObject()
{
    return ((m_p_qml_app_engine && m_p_qml_app_engine->rootObjects().size()) ? m_p_qml_app_engine->rootObjects()[0] : nullptr);
}