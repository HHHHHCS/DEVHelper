#pragma once

#include <thread>
#include <atomic>

#include <QObject>
#include <QMap>
#include <QString>

#include "manager_collection.h"
#include "serial_link.h"


namespace managers
{
    class LinkManager : public Manager
    {
        Q_OBJECT

        signals:
            void linkInfoMap(const QVariantMap link_info_list);
            void linkAdded();
            void linkRemoved();

        private slots:
            void linkDisconnected();

        public:
            Q_INVOKABLE void createChoiceLink(const QString name); // 创建所选连接

            Q_ENUM(communication::Link::LinkType);

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

            // void setConnectionsSuspended(QString reason);
            // void setConnectionsAllowed() { m_connections_suspended = false; }

            QVariantMap getLinkInfoMap() const { return m_link_info_map; }
            int getLinkNum() const { return m_links_list.count(); }
            communication::SharedLinkPtr getSharedLinkPtrByLinkName(const QString name);

            void createScanLinksListWork();
            bool isBelong2LinksList(communication::Link* link);
            void disconnectAll();

        private:
            static constexpr char *LINK_DEV_NAME_LIST[] = {"rc", "rn", "rp", "ba", "uav"};   // 可选设备列表

            std::thread *m_p_serial_scan_thread;
            std::atomic_bool m_serial_scan_thread_stop_flag;

            QVariantMap m_link_info_map;   // 可连接端口名列表

            QList<communication::SharedLinkPtr> m_links_list;   // 选择的连接端口指针对象

            // bool m_connections_suspended;
    };
}   // namespace managers