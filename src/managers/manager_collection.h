#pragma once


#include <QApplication>

#include "link_manager/link_manager.h"
#include "main_manager/main_manager.h"
#include "parameters_manager/parameters_manager.h"
#include "upgrade_manager/upgrade_manager.h"


namespace managers
{
    class ManagerCollection
    {
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
    };
}   // namespace managers