#pragma once

#include <QObject>

#include "manager_collection.h"


namespace managers
{
    class UpgradeManager : public Manager
    {
        Q_OBJECT

        public:
            UpgradeManager(QApplication *app, ManagerCollection* man_collect);
            ~UpgradeManager();

        private:

    };
}   // namespace managers