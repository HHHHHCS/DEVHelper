﻿#pragma once

#include <QStringList>

#include "q_serial.h"
#include "link.h"


namespace communication
{
    class SerialLink : public Link
    {
        Q_OBJECT

        public slots:
            /**
             * @brief 连接错误槽
             * @note 对应信号 QSerialPort::error
             * @param error 具体错误
             */
            void slotLinkError(const port_lib::SerialPort::QSerialPortError&);

        private slots:
            /**
             * @brief 字节发送槽
             * @note 对应信号 communication::Link::sigPacked
             * @param[in] 需要发送的字节
             */
            void slotWriteBytes(const QByteArray &data) override;

            /**
             * @brief 字节接收槽
             * @note 对应信号 QSerialPort::readyRead
             */
            void slotReadBytes();

        public:
            Q_PROPERTY(qint32       baudrate             READ getBaudrate            CONSTANT);
            Q_PROPERTY(qint8        databits             READ getDatabits            CONSTANT);
            Q_PROPERTY(qint8        stopbits             READ getStopbits            CONSTANT);
            Q_PROPERTY(qint8        parity               READ getParity              CONSTANT);

        public:
            explicit SerialLink(const QString &port_name,
                        qint32 baud_rate = 115200,
                        qint8 data_bits = 8,
                        qint8 parity = 0,
                        qint8 stop_bits = 1,
                        qint8 flow_control = 0);
            SerialLink(const SerialLink&) = delete;
            SerialLink(const SerialLink&&) = delete;
            SerialLink& operator=(const SerialLink&) = delete;
            SerialLink& operator=(const SerialLink&&) = delete;
            ~SerialLink() override;

            void setBaudrate(qint32 baud_rate) { if(m_port_ptr) { m_port_ptr->setBaudRate(baud_rate); } }
            void setDatabits(qint8 data_bits) {  if(m_port_ptr) { m_port_ptr->setDataBits(static_cast<port_lib::SerialPort::QDataBits>(data_bits)); } }
            void setStopbits(qint8 stop_bits) { if(m_port_ptr) { m_port_ptr->setStopBits(static_cast<port_lib::SerialPort::QStopBits>(stop_bits)); } }
            void setParity(qint8 parity) { if(m_port_ptr) { m_port_ptr->setParity(static_cast<port_lib::SerialPort::QParity>(parity)); } }
            void setPortName(const QString name) override { if(m_port_ptr) { m_port_ptr->setPortName(name); } }

            QString getPortName() const override { return (m_port_ptr? m_port_ptr->portName() : ""); }
            bool getConnected() const override { return (m_port_ptr ? m_port_ptr->isOpen() : false); }
            qint32 getBaudrate() const { return (m_port_ptr ? m_port_ptr->baudRate() : 0); }
            port_lib::SerialPort::QDataBits getDatabits() const { return (m_port_ptr ? m_port_ptr->dataBits() : port_lib::SerialPort::QDataBits::UnknownDataBits); }
            port_lib::SerialPort::QStopBits getStopbits() const { return (m_port_ptr ? m_port_ptr->stopBits() : port_lib::SerialPort::QStopBits::UnknownStopBits); }
            port_lib::SerialPort::QParity getParity() const { return (m_port_ptr ? m_port_ptr->parity() : port_lib::SerialPort::QParity::UnknownParity); }

            void disconnect() override;

            static QList<port_lib::PortInfoStru> findPortsListForward();

        private:
            using UniquePtrSerialPort = std::unique_ptr<port_lib::SerialPort>;

        private:
            UniquePtrSerialPort m_port_ptr;

            QMutex m_write_mutex;
            QMutex m_read_mutex;

            bool connect() override;
    };
}   // namespace communication