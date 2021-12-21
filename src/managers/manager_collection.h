#pragma once

#include <QObject>


namespace managers
{
    class LinkManager;
    class MainManager;
    class ParametersManager;
    class UpgradeManager;
    class ManagerCollection : public QObject
    {
        Q_OBJECT

        public:
            ManagerCollection(QApplication *app);
            ~ManagerCollection();

            LinkManager* link_man() const { return m_p_link_man; }
            MainManager* main_man() const { return m_p_main_man; }
            ParametersManager* param_man() const { return m_p_param_man; }
            UpgradeManager* upgrade_man() const { return m_p_upgrade_man; }

        private:
            QApplication *m_p_app;
            LinkManager *m_p_link_man;
            MainManager *m_p_main_man;
            ParametersManager *m_p_param_man;
            UpgradeManager *m_p_upgrade_man;

            friend class DevHelperApplication;
    };

    class Manager : public QObject
    {
        Q_OBJECT

        public:
            Manager(QApplication *app, ManagerCollection *man_collect);
            virtual ~Manager();

        protected:
            QApplication *m_app;
            ManagerCollection *m_man_collect;
    };
}   // namespace managers