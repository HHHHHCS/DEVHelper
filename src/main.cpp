#include <QApplication>

#include "dev_helper_application.h"


using namespace dev_helper;
using namespace agent;

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    int exit_code = 0;

    DevHelperApplication *p_app= new DevHelperApplication(argc, argv);
    Q_CHECK_PTR(p_app);

    if(!p_app->bootMainWindow())
    {
        return -1;
    }

    exit_code = p_app->exec();

    p_app->shutdown();
    delete p_app;

    return exit_code;
}
