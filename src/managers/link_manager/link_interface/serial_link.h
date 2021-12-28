#pragma once

#include <QStringList>

#include "q_serial.h"
#include "link.h"


namespace communication
{
    class SerialLink : public Link
    {
        Q_OBJECT

        // public signals:

        // public slots:
        //     void linkError();

        private slots:
            void writeBytes(const QByteArray &data) override;
            void readBytes(QByteArray& data);

        public:
            Q_PROPERTY(qint32       baudrate             READ getBaudrate            CONSTANT);
            Q_PROPERTY(qint8        databits             READ getDatabits            CONSTANT);
            Q_PROPERTY(qint8        stopbits             READ getStopbits            CONSTANT);
            Q_PROPERTY(qint8        parity               READ getParity              CONSTANT);

            Q_INVOKABLE void disconnect() override;

        public:
            SerialLink();
            ~SerialLink();

            void setPortName(const QString name) { if(m_port) { m_port->setPortName(name); } }

            qint32 getBaudrate() const { return (m_port ? m_port->baudRate() : 0); }
            port_lib::SerialPort::QDataBits getDatabits() const { return (m_port ? m_port->dataBits() : port_lib::SerialPort::QDataBits::UnknownDataBits); }
            port_lib::SerialPort::QStopBits getStopbits() const { return (m_port ? m_port->stopBits() : port_lib::SerialPort::QStopBits::UnknownStopBits); }
            port_lib::SerialPort::QParity getParity() const { return (m_port ? m_port->parity() : port_lib::SerialPort::QParity::UnknownParity); }

            bool getConnected() const override { return (m_port ? m_port->isOpen() : false); }

            static QList<port_lib::PortInfoStru> findPortsListForward();

        protected:
            void connectionRemoved();

        private:
            QThread *m_thread;
            port_lib::SerialPort *m_port;

            bool connect() override;
    };
}   // namespace communication