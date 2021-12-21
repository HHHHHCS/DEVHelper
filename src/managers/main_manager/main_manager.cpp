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