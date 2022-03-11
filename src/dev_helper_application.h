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
    class DevHelperApplication final : public QApplication
    {
        Q_OBJECT

        public:
            explicit DevHelperApplication(int &argc, char* argv[]);
            DevHelperApplication(const DevHelperApplication&) = delete;
            DevHelperApplication(const DevHelperApplication&&) = delete;
            DevHelperApplication& operator=(const DevHelperApplication&) = delete;
            DevHelperApplication& operator=(const DevHelperApplication&&) = delete;
            ~DevHelperApplication() final = default;

            std::shared_ptr<::managers::ManagerCollection> man_collection() const { return m_p_man_collect; }

        public:
            void initCommonType();
            bool bootMainWindow();

            void shutdown();

        private:
            std::shared_ptr<QQmlApplicationEngine> m_p_qml_app_engine;
            std::shared_ptr<::managers::ManagerCollection> m_p_man_collect;

            QObject* rootQmlObject();
    };
}   // namespace app
}   // namespace dev_helper