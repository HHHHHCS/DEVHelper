#pragma once

#include <QObject>


namespace managers
{
    class ManagerCollection;
    class ParametersManager : public QObject
    {
        Q_OBJECT

        public:
            ParametersManager(ManagerCollection* man_collect);
            ~ParametersManager();

        private:
            ManagerCollection* m_p_manager_collect;
    };
}   // namespace managers