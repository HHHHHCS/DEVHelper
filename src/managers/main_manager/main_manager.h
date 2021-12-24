#pragma once

#include <QObject>
#include <QQmlApplicationEngine>

#include "manager_collection.h"


namespace managers
{
    class MainManager : public Manager
    {
        Q_OBJECT

        public:
            MainManager(QApplication *app, ManagerCollection* man_collect);
            ~MainManager();

            /** 
             * @brief 创建QML应用引擎
             */
            QQmlApplicationEngine* createQMLAppEngine(QObject *parent = nullptr);

            /** 
             * @brief 创建QML初始窗口
             */
            void createMainWindow(QQmlApplicationEngine *qml_engine = nullptr);

            // TODO(hunagchsh): 增加Image Provider

        private:

    };

    // TODO(huangchsh): 实现总控件属性控制类
}   // namespace managers