#pragma once

#include <QApplication>
#include <QQmlApplicationEngine>

#include <QDebug>


namespace dev_helper
{
namespace app
{
    class DevHelperApplication : public QApplication
    {
        Q_OBJECT

        public:
            explicit DevHelperApplication() = default;
            DevHelperApplication(int &argc, char* argv[]);
            ~DevHelperApplication();

        public:
            static DevHelperApplication* m_s_app_inst;

            bool bootMainWindow();

            void shutdown();

        private:
            QQmlApplicationEngine *m_qml_qpp_engine;
    };

    DevHelperApplication* devHelperApp(void);
}   // namespace dev_helper
}   // namespace agent