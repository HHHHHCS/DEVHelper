#pragma once

#include <QObject>

#include "manager_collection.h"
#include "serial_link.h"


namespace dev_helper { namespace app { class DevHelperApplication; } }

namespace managers
{
    class LinkManager : public Manager
    {
        Q_OBJECT

        signals:

        private slots:
            // void linkDisconnected();

        public:
            Q_PROPERTY(QStringList port_name_list       READ getPortNameList    CONSTANT);
            Q_PROPERTY(QString     choice_port_name     READ getChoicePortName  WRITE setChoicePortName);
            // Q_PROPERTY(QStringList          linkTypeStrings         READ getLinkTypeStrings        CONSTANT)

            // Q_INVOKABLE void createConnectedLink(communication::LinkConfiguration* cfg);
            // Q_INVOKABLE void shutdown();

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

            void setChoicePortName(const QString name) { m_choice_port_name = name; }
            // void setConnectionsSuspended(QString reason);
            // void setConnectionsAllowed() { m_connections_suspended = false; }

            QStringList getPortNameList() const { return m_port_name_list; }
            QString getChoicePortName() const { return m_choice_port_name; }
            // QStringList getLinkTypeStrings() const;

            void createScanLinksListWork();
            // bool createConnectedLink(communication::SharedLinkConfigurationPtr& cfg);
            // void disconnectAll();

        private:
            QStringList m_port_name_list;   // 可连接端口名列表
            QString m_choice_port_name;     // 选择的连接端口名

            QList<communication::SharedLinkPtr> m_links_list;   // 选择的连接端口指针对象

            // bool m_connections_suspended;

    };
}   // namespace managers