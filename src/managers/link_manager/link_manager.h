#pragma once

#include <QList>
#include <QMutex>
#include <QObject>

#include "manager_collection.h"
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
        //     void portStringsChanged();
        //     void portsChanged();

        private Q_SLOTS:
        //     void linkDisconnected();

        public:
        //     Q_PROPERTY(QStringList          linkTypeStrings         READ getLinkTypeStrings        CONSTANT)
        //     Q_PROPERTY(QStringList          serialBaudRates         READ getSerialBaudrates        CONSTANT)
        //     Q_PROPERTY(QStringList          serialPortStrings       READ getSerialPortStrings      NOTIFY portStringsChanged)
        //     Q_PROPERTY(QStringList          serialPorts             READ getSerialPorts            NOTIFY portsChanged)

        //     Q_INVOKABLE void createConnectedLink(communication::LinkConfiguration* cfg);
        //     Q_INVOKABLE void shutdown();

        public:
            LinkManager(QApplication *app, ManagerCollection* man_collect);
            ~LinkManager();

        //     QList<communication::SharedLinkPtr> getLinksList() { return m_links_list; }
        //     QStringList getLinkTypeStrings() const;
        //     QStringList getSerialBaudrates();
        //     QStringList getSerialPortStrings();
        //     QStringList getSerialPorts();

            int foundLinksList();

        //     void setConnectionsSuspended(QString reason);
        //     void setConnectionsAllowed() { m_connections_suspended = false; }

        //     bool createConnectedLink(communication::SharedLinkConfigurationPtr& cfg);
        //     void disconnectAll();

        private:
        //     QList<communication::SharedLinkPtr> m_links_list;

        //     bool m_connections_suspended;

    };
}   // namespace managers