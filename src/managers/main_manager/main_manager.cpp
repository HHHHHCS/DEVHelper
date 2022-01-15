#include <QQmlContext>

#include "link_manager.h"
#include "parameters_manager.h"
#include "upgrade_manager.h"
#include "main_manager.h"


using namespace managers;

MainManager::MainManager(QApplication *app, ManagerCollection* man_collect)
    : Manager(app, man_collect)
{
    QQmlEngine::setObjectOwnership(this, QQmlEngine::CppOwnership);
}

MainManager::~MainManager()
{
}

/**
 * @brief 创建QML应用引擎
 */
QQmlApplicationEngine* MainManager::createQMLAppEngine(QObject *parent /* = nullptr */)
{
    QQmlApplicationEngine *p_qml_app_engine = new QQmlApplicationEngine(parent);

    p_qml_app_engine->addImportPath("qrc:/QML");
    // 设置根对象属性，用于UI交互
    p_qml_app_engine->rootContext()->setContextProperty("link_manager_obj", man_collect()->link_man());
    p_qml_app_engine->rootContext()->setContextProperty("param_manager_obj", man_collect()->param_man());
    p_qml_app_engine->rootContext()->setContextProperty("upgrade_manager_obj", man_collect()->upgrade_man());

    return p_qml_app_engine;
}

/**
 * @brief 创建QML初始窗口
 */
void MainManager::createMainWindow(QQmlApplicationEngine *qml_engine)
{
    Q_CHECK_PTR(qml_engine);
    qml_engine->load(QUrl(QStringLiteral("qrc:/QML/main.qml")));
}