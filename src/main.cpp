#include <QApplication>

#include "dev_helper_application.h"


using namespace dev_helper;
using namespace app;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    int exit_code = 0;

    std::unique_ptr<DevHelperApplication> p_app= std::make_unique<DevHelperApplication>(argc, argv);
    Q_CHECK_PTR(p_app);

    p_app->initCommonType();

    if(!p_app->bootMainWindow())
    {
        return -1;
    }

    exit_code = p_app->exec();

    p_app->shutdown();

    return exit_code;
}
