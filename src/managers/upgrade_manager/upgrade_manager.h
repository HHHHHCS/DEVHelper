#pragma once

#include <QObject>


namespace managers
{
    class ManagerCollection;
    class UpgradeManager : public QObject
    {
        Q_OBJECT

        public:
            UpgradeManager(ManagerCollection* man_collect);
            ~UpgradeManager();

        private:
            ManagerCollection* m_p_manager_collect;
    };
}   // namespace managers