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
    : Link(LinkType::SERIAL)
    , m_write_mutex()
    , m_read_mutex()
{
    m_port_ptr = std::make_unique<SerialPort>(port_name, baud_rate,
                            (SerialPort::QDataBits)data_bits,
                            (SerialPort::QParity)parity,
                            (SerialPort::QStopBits)stop_bits,
                            (SerialPort::QFlowControl)flow_control);
    Q_CHECK_PTR(m_port_ptr);

    QObject::connect(m_port_ptr.get(), static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error), this, &SerialLink::slotLinkError);
    QObject::connect(m_port_ptr.get(), &QSerialPort::readyRead, this, &SerialLink::slotReadBytes);
}

SerialLink::~SerialLink()
{
    if(getConnected())
    {
        disconnect();
    }
}

bool SerialLink::connect()
{
    if(!m_port_ptr)
    {
        return false;
    }

    if(m_port_ptr->openPort())
    {
        qDebug("Succeed to set port ");

        emit sigConnected();
        return true;
    }

    return false;
}

void SerialLink::disconnect()
{
    if(!m_port_ptr)
    {
        return;
    }

    m_port_ptr->closePort();

    emit sigDisconnected();
}

/**
 * @brief 字节发送槽
 * @note 对应信号 communication::Link::sigPacked
 * @param[in] 需要发送的字节
 */
void SerialLink::slotWriteBytes(const QByteArray &data)
{
    if(!m_port_ptr)
    {
        return;
    }

    QMutexLocker locker(&m_write_mutex);
    qint64 sent_size = m_port_ptr->writePort(data);
    if(0 < sent_size)
    {
        // TODO(huangchsh): 此处可扩展
        sigBytesSent(sent_size);
    }
}

/**
 * @brief 字节接收槽
 * @note 对应信号 QSerialPort::readyRead
 */
void SerialLink::slotReadBytes()
{
    if(!m_port_ptr)
    {
        return;
    }

    QMutexLocker locker(&m_read_mutex);
    QByteArray data;
    if(0 < m_port_ptr->readPort(data))
    {
        emit sigBytesReceived(data);
    }
}

/**
 * @brief 连接错误槽
 * @note 对应信号 QSerialPort::error
 * @param error 具体错误
 */
void SerialLink::slotLinkError(const SerialPort::QSerialPortError& error)
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

    emit sigCommError(getPortName(), QString(error));
}

QList<port_lib::PortInfoStru> SerialLink::findPortsListForward()
{
    return port_lib::findPortsList();
}