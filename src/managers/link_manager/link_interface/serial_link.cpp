#include <QDebug>

#include "serial_link.h"


using namespace communication;

SerialLink::SerialLink()
    : Link()
{
    m_port = new port_lib::SerialPort();
    Q_CHECK_PTR(m_port);

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

    // TODO(huangchsh): 等待选择信号，传入端口名，开启端口
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
    return port_lib::SerialPort::findPortsList();
}