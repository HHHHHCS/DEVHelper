#pragma once

#include <QObject>

#include "../manager_collection.h"


namespace managers
{
    class ParametersManager : public Manager
    {
        Q_OBJECT

        public:
            ParametersManager(QApplication *app, ManagerCollection* man_collect);
            ~ParametersManager();

        private:

    };
}   // namespace managers