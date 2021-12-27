#pragma once

#include <atomic>

#include <QStringList>

#include "link.h"


namespace communication
{
    class SerialPort;
    class SerialLink : public Link
    {
        Q_OBJECT

        public:
            Q_PROPERTY(bool        is_find_ports_list   READ getFindPortsListFlag)
            Q_PROPERTY(QStringList port_name_list       READ getPortNameList);

            Q_INVOKABLE void disconnect() override;

        public Q_SLOTS:
            void linkError()

        private Q_SLOTS:
            void writeBytes(const QByteArray &data) override;
            void readBytes(const QByteArray& data);

        public:
            SerialLink();
            ~SerialLink();

            bool getConnected() const override { return (m_port ? m_port->isOpen() : false); }
            bool getFindPortsListFlag() const override { return m_is_find_ports_list.load(); }
            QStringList getPortNameList() const override { return m_port_name_list; }

            void disconnect() override;

        protected:
            void connectionRemoved();

        private:
            SerialPort *m_port;

            std::atomic_bool m_is_find_ports_list;  // 找到可连接端口的标志

            QStringList m_port_name_list;   // 可连接端口名列表

            bool connect() override;
    }
}   // namespace communication