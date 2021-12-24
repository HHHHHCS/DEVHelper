#pragma once

#include <QList>
#include <QMutex>
#include <QObject>

#include "manager_collection.h"
#include "link_interface/link_configuration.h"
#include "link_interface/link.h"


namespace managers
{
namespace dev_helper
{
namespace app
{
    class DevHelperApplication;
}
}
    class LinkManager : public Manager
    {
        Q_OBJECT

        Q_SIGNALS:
            void portStringsChanged();
            void portsChanged();

        private Q_SLOTS:
            void linkDisconnected();

        public:
            Q_INVOKABLE void createConnectedLink(communication::LinkConfiguration* cfg);
            Q_INVOKABLE communication::LinkConfiguration* createConfiguration(communication::LinkConfiguration::LinkType type, const QString &name);
            Q_INVOKABLE communication::LinkConfiguration* startConfigurationEditing(communication::LinkConfiguration* cfg);
            Q_INVOKABLE void cancelConfigurationEditing(communication::LinkConfiguration* cfg);
            Q_INVOKABLE bool endConfigurationEditing(communication::LinkConfiguration* cfg, communication::LinkConfiguration* edit_cfg);
            Q_INVOKABLE bool endCreateConfiguration(communication::LinkConfiguration* cfg);
            Q_INVOKABLE void removeConfiguration(communication::LinkConfiguration* cfg);
            Q_INVOKABLE void shutdown();

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

            QList<communication::SharedLinkPtr> getLinksList() { return m_links_list; }
            QStringList getLinkTypeStrings() const;
            QStringList getSerialBaudrates();
            QStringList getSerialPortStrings();
            QStringList getSerialPorts();

            void loadLinkConfigurationList();
            void saveLinkConfigurationList();

            void suspendConfigurationUpdates(bool suspend);

            void setConnectionsSuspended(QString reason);
            void setConnectionsAllowed() { m_connections_suspended = false; }

            bool createConnectedLink(communication::SharedLinkConfigurationPtr& cfg);
            void disconnectAll();
            // communication::SharedLinkPtr mavlinkForwardingLink();

        private:
            QList<communication::SharedLinkPtr> m_links_list;

            bool m_connections_suspended;
            // QmlObjectListModel m_qml_configurations;

            // QmlObjectListModel* qmlLinkConfigurations() { return &m_qml_configurations; }
    };
}   // namespace dev_helper