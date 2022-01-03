#include <QDebug>

#include "serial_link.h"


using namespace communication;
using namespace port_lib;

SerialLink::SerialLink(const QString &port_name,
                        qint32 baud_rate /* = 115200 */,
                        qint8 data_bits /* = 8 */,
                        qint8 parity /* = 0 */,
                        qint8 stop_bits /* = 1 */,
                        qint8 flow_control /* = 0 */)
    : Link()
{
    m_port = new SerialPort(port_name, baud_rate,
                            (SerialPort::QDataBits)data_bits,
                            (SerialPort::QParity)parity,
                            (SerialPort::QStopBits)stop_bits,
                            (SerialPort::QFlowControl)flow_control);
    Q_CHECK_PTR(m_port);

    QObject::connect(m_port, static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &SerialLink::linkError);
    QObject::connect(m_port, &QIODevice::readyRead, this, &SerialLink::readBytes);
}

SerialLink::~SerialLink()
{
    if(getConnected())
    {
        disconnect();
    }

    m_port = nullptr;
}

void SerialLink::writeBytes(const QByteArray &data)
{
    if(!m_port)
    {
        return;
    }

    emit bytesSent(this, data);
    m_port->writePort(data);
}

void SerialLink::readBytes()
{
    if(!m_port)
    {
        return;
    }

    QByteArray data;
    m_port->readPort(data);

    emit bytesReceived(this, data);
}

bool SerialLink::connect()
{
    if(!m_port)
    {
        return false;
    }

    if(m_port->openPort())
    {
        qDebug("Succeed to set port ");
        return true;
    }

    emit connected();

    return false;
}

void SerialLink::disconnect()
{
    if(!m_port)
    {
        return;
    }

    m_port->closePort();

    emit disconnected();
}

void SerialLink::linkError(SerialPort::QSerialPortError error)
{
    switch (error)
    {
        case QSerialPort::NoError:
        {
            break;
        }
        case QSerialPort::ResourceError:
        {
            disconnect();
            break;
        }
        default:
        {
            // 用于调试，观察错误
            qDebug() << "SerialLink::linkError" << error;
            break;
        }
    }
}

QList<port_lib::PortInfoStru> SerialLink::findPortsListForward()
{
    return SerialPort::findPortsList();
}