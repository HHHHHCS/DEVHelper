#include <QDebug>

#include "q_serial.h"


using namespace port_lib;

SerialPort::SerialPort(const QString &port_name,
                        qint32 baud_rate /* = 115200 */,
                        QDataBits data_bits /* = QDataBits::Data8 */,
                        QParity parity /* = QParity::NoParity */,
                        QStopBits stop_bits /* = QStopBits::OneStop */,
                        QFlowControl flow_control /* = QFlowControl::NoFlowControl */)
    : QSerialPort()
{
    if(baud_rate < 0)
    {
        qFatal("Error baud rate: %d", baud_rate);
        return;
    }

    this->setPortName(port_name);
    this->setBaudRate(baud_rate);
    this->setDataBits(data_bits);
    this->setParity(parity);
    this->setStopBits(stop_bits);
    this->setFlowControl(flow_control);
}

SerialPort::SerialPort(const QString &port_name)
    : QSerialPort()
{
    this->setPortName(port_name);
}

SerialPort::SerialPort(const QString &port_name, qint32 baud_rate /* = 115200  */)
    : QSerialPort()
{
    this->setPortName(port_name);
    this->setBaudRate(baud_rate);
}

SerialPort::~SerialPort()
{
    closePort();
}

bool SerialPort::openPort()
{
    if(this->isOpen())
    {
        quint8 retry_times = 5;
        while(retry_times--)
        {
            auto ret = this->open(QIODevice::ReadWrite);
            if(ret)
            {
                this->setDataTerminalReady(true);

                qInfo("Succeed to open port, finally retry time %d.", (5 - retry_times));
                return true;
            }
            qWarning("Tryying to open port, retry time %d.", (5 - retry_times));
        }
    }

    qWarning("Port is open now, do not need to open again.");

    return false;
}

void SerialPort::closePort()
{
    if(!this->isOpen())
    {
        qDebug("Port had been closed.");
        return;
    }

    this->close();
    this->deleteLater();
}

qint64 SerialPort::readPort(QByteArray &buffer)
{
    if(!this->isOpen())
    {
        qDebug("Failed to read, because port is closed.");
        return -2;
    }

    QMutexLocker locker(&m_read_mutex);
    qint64 byte_counts = this->bytesAvailable();
    if(byte_counts)
    {
        buffer = this->readAll();
    }

    return buffer.count();
}

qint64 SerialPort::writePort(const QByteArray &buffer)
{
    if(!this->isOpen())
    {
        qDebug("Failed to read, because port is closed.");
        return -2;
    }

    QMutexLocker locker(&m_write_mutex);
    auto send_counts = this->write(buffer);
    qDebug() << "Send bytes:" << buffer;

    return send_counts;
}

QList<PortInfoStru> SerialPort::findPortsList()
{
    QList<PortInfoStru> ports_name_list;

    const auto list = QSerialPortInfo::availablePorts();
    if(!list.empty())
    {
        for(auto &info : list)
        {
            PortInfoStru port_info_stru;

            port_info_stru.name = info.portName();
            port_info_stru.system_location = info.systemLocation();
            port_info_stru.description = info.description();
            port_info_stru.manufacturer = info.manufacturer();
            port_info_stru.serial_number = info.serialNumber();

            if(info.hasVendorIdentifier())
            {
                port_info_stru.vendor_identifier = info.vendorIdentifier();
            }

            if(info.hasProductIdentifier())
            {
                port_info_stru.product_identifier = info.productIdentifier();
            }

            ports_name_list.append(port_info_stru);
        }
    }

    return ports_name_list;
}