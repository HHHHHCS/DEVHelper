#pragma once

#include <thread>
#include <atomic>

#include <QObject>
#include <QStringList>

#include "manager_collection.h"
#include "serial_link.h"


namespace managers
{
    class LinkManager : public Manager
    {
        Q_OBJECT

        signals:
            void portNameList(const QStringList link_list);

        private slots:
            void linkDisconnected();

        public:
            Q_PROPERTY(QStringList portNameList       READ getPortNameList    CONSTANT);                  // 可连接端口列表
            Q_PROPERTY(QString     choicePortName     READ getChoicePortName  WRITE createChoiceLink);    // 选择连接的端口
            // Q_PROPERTY(QStringList          linkTypeStrings         READ getLinkTypeStrings        CONSTANT);    // 可连接端口类型列表

            // Q_INVOKABLE void createConnectedLink(communication::LinkConfiguration* cfg);
            // Q_INVOKABLE void shutdown();

            enum class LinkType : int8_t
            {
                UNKNOWN_TYPE = -1,
                CAN,
                SERIAL,
                TCP,
                UDP,
            };
            Q_ENUM(LinkType);

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

            // void setConnectionsSuspended(QString reason);
            // void setConnectionsAllowed() { m_connections_suspended = false; }

            QStringList getPortNameList() const { return m_port_name_list; }
            QString getChoicePortName() const { return m_choice_port_name; }
            int getLinkNum() const { return m_links_list.count(); }
            // QStringList getLinkTypeStrings() const;
            communication::SharedLinkPtr getSharedLinkPtrByPortName(const QString name);

            void createScanLinksListWork();
            void createChoiceLink(const QString name);
            bool isBelong2LinksList(communication::Link* link);
            void disconnectAll();

        private:
            static constexpr char *LINK_DEV_NAME_LIST[] = {"rc", "rn", "rp", "ba", "uav"};   // 可选设备列表

            std::thread *m_p_serial_scan_thread;
            std::atomic_bool m_serial_scan_thread_stop_flag;

            QStringList m_port_name_list;   // 可连接端口名列表
            QString m_choice_port_name;     // 选择的连接端口名

            QList<port_lib::PortInfoStru> m_links_info_list;
            QList<communication::SharedLinkPtr> m_links_list;   // 选择的连接端口指针对象

            // bool m_connections_suspended;
    };
}   // namespace managers