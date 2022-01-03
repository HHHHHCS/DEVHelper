#pragma once

#include <QApplication>
#include <QDebug>
#include <QQmlApplicationEngine>

#include "manager_collection.h"


namespace managers { class ManagerCollection; }

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

            ::managers::ManagerCollection *man_collection() const { return m_p_man_collect; }

        public:
            void initCommonType();
            bool bootMainWindow();

            void shutdown();

        private:
            QQmlApplicationEngine *m_p_qml_app_engine;
            ::managers::ManagerCollection *m_p_man_collect;

            QObject* rootQmlObject();
    };
}   // namespace app
}   // namespace dev_helper