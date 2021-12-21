#pragma once

#include <QObject>


namespace managers
{
    class ManagerCollection;
    class LinkManager : public QObject
    {
        Q_OBJECT

        public:
            LinkManager(ManagerCollection* man_collect);
            ~LinkManager();

        private:
            ManagerCollection* m_p_manager_collect;
    };
}   // namespace dev_helper