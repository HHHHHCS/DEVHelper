#pragma once

#include <QObject>

#include "manager_collection.h"


namespace managers
{
    class LinkManager : public Manager
    {
        Q_OBJECT

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

        private:

    };
}   // namespace dev_helper