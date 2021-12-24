#pragma once

#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>

#include "manager_collection.h"


namespace dev_helper
{
namespace app
{
    class ::managers::ManagerCollection;
    class DevHelperApplication : public QApplication
    {
        Q_OBJECT

        public:
            explicit DevHelperApplication() = default;
            DevHelperApplication(int &argc, char* argv[]);
            ~DevHelperApplication();

            ::managers::ManagerCollection *man_collection() const { return m_p_man_collect; }

        public:
            static DevHelperApplication* m_s_app_inst;

            bool bootMainWindow();

            void shutdown();

        private:
            QQmlApplicationEngine *m_p_qml_app_engine;
            ::managers::ManagerCollection *m_p_man_collect;
    };

    DevHelperApplication* devHelperApp(void);
}   // namespace app
}   // namespace dev_helper