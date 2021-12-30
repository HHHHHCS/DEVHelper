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

    // TODO(huangchsh): 信号与槽连接
    // TODO(huangchsh): 建立收发线程处理信号与槽
    QThread* m_thread = new QThread(this);
    this->moveToThread(m_thread);
}

SerialLink::~SerialLink()
{
    if(!getConnected())
    {
        m_port = nullptr;
    }

    disconnect();

    m_thread = nullptr;
    m_port = nullptr;
}

void SerialLink::writeBytes(const QByteArray &data)
{
    if(!m_port)
    {
        return;
    }

    m_port->writePort(data);
}

void SerialLink::readBytes(QByteArray &data)
{
    if(!m_port)
    {
        return;
    }

    m_port->readPort(data);
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

    return false;
}

void SerialLink::disconnect()
{
    if(!m_port)
    {
        return;
    }

    m_port->closePort();
}

QList<port_lib::PortInfoStru> SerialLink::findPortsListForward()
{
    return SerialPort::findPortsList();
}