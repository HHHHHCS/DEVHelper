#pragma once

#include <QObject>


namespace managers
{
    class ManagerCollection;
    class MainManager : public QObject
    {
        Q_OBJECT

        public:
            MainManager(ManagerCollection* man_collect);
            ~MainManager();

        private:
            ManagerCollection* m_p_manager_collect;
    };
}   // namespace managers