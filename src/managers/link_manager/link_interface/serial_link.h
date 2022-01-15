#pragma once

#include <QStringList>

#include "q_serial.h"
#include "link.h"


namespace communication
{
    class SerialLink : public Link
    {
        Q_OBJECT

        public slots:
            void linkError(port_lib::SerialPort::QSerialPortError error);

        private slots:
            void writeBytes(const QByteArray &data) override;
            void readBytes();

        public:
            Q_PROPERTY(qint32       baudrate             READ getBaudrate            CONSTANT);
            Q_PROPERTY(qint8        databits             READ getDatabits            CONSTANT);
            Q_PROPERTY(qint8        stopbits             READ getStopbits            CONSTANT);
            Q_PROPERTY(qint8        parity               READ getParity              CONSTANT);

            Q_INVOKABLE void disconnect() override;

        public:
            SerialLink(const QString &port_name,
                        qint32 baud_rate = 115200,
                        qint8 data_bits = 8,
                        qint8 parity = 0,
                        qint8 stop_bits = 1,
                        qint8 flow_control = 0);
            ~SerialLink();

            void setBaudrate(qint32 baud_rate) { if(m_port) { m_port->setBaudRate(baud_rate); } }
            void setDatabits(qint8 data_bits) {  if(m_port) { m_port->setDataBits(static_cast<port_lib::SerialPort::QDataBits>(data_bits)); } }
            void setStopbits(qint8 stop_bits) { if(m_port) { m_port->setStopBits(static_cast<port_lib::SerialPort::QStopBits>(stop_bits)); } }
            void setParity(qint8 parity) { if(m_port) { m_port->setParity(static_cast<port_lib::SerialPort::QParity>(parity)); } }
            void setPortName(const QString name) override { if(m_port) { m_port->setPortName(name); } }

            QString getPortName() const override { return (m_port? m_port->portName() : ""); }
            bool getConnected() const override { return (m_port ? m_port->isOpen() : false); }
            qint32 getBaudrate() const { return (m_port ? m_port->baudRate() : 0); }
            port_lib::SerialPort::QDataBits getDatabits() const { return (m_port ? m_port->dataBits() : port_lib::SerialPort::QDataBits::UnknownDataBits); }
            port_lib::SerialPort::QStopBits getStopbits() const { return (m_port ? m_port->stopBits() : port_lib::SerialPort::QStopBits::UnknownStopBits); }
            port_lib::SerialPort::QParity getParity() const { return (m_port ? m_port->parity() : port_lib::SerialPort::QParity::UnknownParity); }

            static QList<port_lib::PortInfoStru> findPortsListForward();

        protected:
            void connectionRemoved();

        private:
            port_lib::SerialPort *m_port;

            bool connect() override;
    };
}   // namespace communication