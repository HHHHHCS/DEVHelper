#pragma once

#include <QMap>
#include <QStringList>
#include <QMutexLocker>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace dev_helper
{

namespace port_lib
{
    class SerialPort : public QSerialPort
    {
        public:
            SerialPort();
            SerialPort(qint32 baud_rate = QSerialPort::Baud115200,
                                DataBits data_bits = QSerialPort::Data8,
                                Parity parity = QSerialPort::NoParity,
                                StopBits stop_bits = QSerialPort::OneStop,
                                FlowControl flow_control = QSerialPort::NoFlowControl);
            SerialPort(const QString &port_name);
            SerialPort(const QString &port_name, qint32 baud_rate);
            ~SerialPort();

            struct PortInfoStru
            {
                explicit PortInfoStru()
                {
                    vendor_identifier = 0;
                    product_identifier = 0;
                    name = "";
                    system_location = "";
                    description = "";
                    manufacturer = "";
                    serial_number = "";
                };

                quint16 vendor_identifier;
                quint16 product_identifier;

                QString name;
                QString system_location;
                QString description;
                QString manufacturer;
                QString serial_number;
            };
            QMap<quint16, PortInfoStru> getPortsList() const { return m_ports_name_map; };

            bool findPortsList();
            bool openPort();
            void closePort();
            qint64 readPort(QByteArray &buffer);
            qint64 writePort(QByteArray &buffer);

        private:
            SerialPort(const SerialPort&);
            SerialPort& operator=(const SerialPort&);

            QMutex m_write_mutex;
            QMutex m_read_mutex;

            QMap<quint16, PortInfoStru> m_ports_name_map;

            bool m_is_port_found;
    };
}

}