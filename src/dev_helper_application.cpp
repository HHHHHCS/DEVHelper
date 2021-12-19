#include "dev_helper_application.h"


using namespace dev_helper;
using namespace app;

DevHelperApplication* DevHelperApplication::m_s_app_inst = nullptr;

DevHelperApplication::DevHelperApplication(int &argc, char* argv[])
    : QApplication(argc, argv)
    , m_qml_qpp_engine(nullptr)
{
    m_s_app_inst = this;
}

DevHelperApplication::~DevHelperApplication()
{
    if(m_qml_qpp_engine)
    {
        delete m_qml_qpp_engine;
    }

    m_qml_qpp_engine = nullptr;
}

void DevHelperApplication::shutdown()
{
    delete m_qml_qpp_engine;
}

bool DevHelperApplication::bootMainWindow()
{
    m_qml_qpp_engine = new QQmlApplicationEngine(this);
    Q_CHECK_PTR(m_qml_qpp_engine);

    m_qml_qpp_engine->load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
    if(m_qml_qpp_engine->rootObjects().isEmpty())
    {
        return false;
    }

    return true;
}

DevHelperApplication* devHelperApp(void)
{
    return DevHelperApplication::m_s_app_inst;
}
