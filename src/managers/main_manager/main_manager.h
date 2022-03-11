#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "manager_collection.h"


namespace managers
{
    class MainManager final : public Manager
    {
        Q_OBJECT

        public:
            explicit MainManager(QApplication *app, ManagerCollection* man_collect);
            MainManager(const MainManager&) = delete;
            MainManager(const MainManager&&) = delete;
            MainManager& operator=(const MainManager&) = delete;
            MainManager& operator=(const MainManager&&) = delete;
            ~MainManager() final = default;

            /**
             * @brief 创建QML应用引擎
             */
            std::shared_ptr<QQmlApplicationEngine> createQMLAppEngine(QObject *parent = nullptr);

            /**
             * @brief 创建QML初始窗口
             */
            void createMainWindow(QQmlApplicationEngine *qml_engine);

            // TODO(hunagchsh): 增加Image Provider
    };
    // TODO(huangchsh): 实现总控件属性控制类
}   // namespace managers