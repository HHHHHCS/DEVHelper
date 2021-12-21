#pragma once

#include <QObject>
#include <QQmlApplicationEngine>


namespace managers
{
    class ManagerCollection;
    class MainManager : public QObject
    {
        Q_OBJECT

        public:
            MainManager(ManagerCollection* man_collect);
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
            ManagerCollection* m_p_manager_collect;

    };

    // TODO(huangchsh): 实现总控件属性控制类
}   // namespace managers