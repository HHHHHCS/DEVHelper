#pragma once


#include <QSerialPort>
#include <QSerialPortInfo>


namespace dev_helper
{

namespace port_lib
{
    class SerialPort : public QSerialPort
    {
        public:
            explicit SerialPort(qint32 baud_rate = QSerialPort::Baud115200,
                                DataBits data_bits = QSerialPort::Data8,
                                Parity parity = QSerialPort::NoParity,
                                StopBits stop_bits = QSerialPort::OneStop,
                                FlowControl flow_control = QSerialPort::NoFlowControl);
            explicit SerialPort(const QString &port_name);
            explicit SerialPort(const QString &port_name, qint32 baud_rate);
            ~SerialPort();

            bool findPort(const QString desc);
            bool openPort();
            void closePort();
            qint64 readPort(QByteArray &buffer);
            qint64 writePort(QByteArray &buffer);

        private:
            SerialPort(const SerialPort&);
            SerialPort& operator=(const SerialPort&);

            bool m_is_port_found;
    };
}

}