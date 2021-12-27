#include <QDebug>

#include "q_serial.h"


using namespace dev_helper;
using namespace dev_helper::port_lib;

SerialPort::SerialPort()
    : QSerialPort()
    , m_is_port_found(false)
{
}

SerialPort::SerialPort(qint32 baud_rate /* = QSerialPort::Baud115200 */,
                    DataBits data_bits /* = QSerialPort::Data8 */,
                    Parity parity /* = QSerialPort::NoParity */,
                    StopBits stop_bits /* = QSerialPort::OneStop */,
                    FlowControl flow_control /* = QSerialPort::NoFlowControl */)
    : QSerialPort()
    , m_is_port_found(false)
{
    if(baud_rate < 0)
    {
        qFatal("Error baud rate: %d", baud_rate);
        return;
    }

    this->setBaudRate(baud_rate);
    this->setDataBits(data_bits);
    this->setParity(parity);
    this->setStopBits(stop_bits);
    this->setFlowControl(flow_control);
}

SerialPort::SerialPort(const QString &port_name)
    : QSerialPort()
    , m_is_port_found(false)
{
    this->setPortName(port_name);
}

SerialPort::SerialPort(const QString &port_name, qint32 baud_rate)
    : QSerialPort()
    , m_is_port_found(false)
{
    this->setPortName(port_name);
    this->setBaudRate(baud_rate);
}

SerialPort::~SerialPort()
{
    closePort();
}

bool SerialPort::findPortsList()
{
    if(this->isOpen())
    {
        qInfo("Port is open now, please close it first.");
    }

    const auto list = QSerialPortInfo::availablePorts();
    if(!list.isEmpty())
    {
        quint16 num = 0;
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

            m_ports_name_map.insert(num, port_info_stru);

            ++num;
        }
        return true;
    }

    return false;
}

bool SerialPort::openPort()
{
    if(!m_is_port_found)
    {
        qDebug("None ports to open.Please find port first.");
        return false;
    }

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
    if(!m_is_port_found)
    {
        qDebug("None ports to close.");
        return;
    }

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
    if(!m_is_port_found)
    {
        qDebug("None ports to read.");
        return -1;
    }

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

qint64 SerialPort::writePort(QByteArray &buffer)
{
    if(!m_is_port_found)
    {
        qDebug("None ports to write.");
        return -1;
    }

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
