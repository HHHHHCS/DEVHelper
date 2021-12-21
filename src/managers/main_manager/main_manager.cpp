#include "main_manager.h"


using namespace managers;

MainManager::MainManager(ManagerCollection* man_collect)
    : QObject(reinterpret_cast<QObject*>(man_collect))
    , m_p_manager_collect(man_collect)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MainManager::~MainManager()
{
    m_p_manager_collect = nullptr;
}

QQmlApplicationEngine* MainManager::createQMLAppEngine(QObject *parent /* = nullptr */)
{
    QQmlApplicationEngine *p_qml_app_engine = new QQmlApplicationEngine(parent);

    p_qml_app_engine->addImportPath("qrc:/QML");
    // TODO(huangchsh): 设置根对象属性

    return p_qml_app_engine;
}

void MainManager::createMainWindow(QQmlApplicationEngine *qml_engine /* = nullptr */)
{
    Q_CHECK_PTR(qml_engine);
    qml_engine->load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
}