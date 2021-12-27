#include <future>

#include <QMap>

#include "serial_link.h"


using namespace communication;

SerialLink::SerialLink()
    : Link()
    , m_port(nullptr)
{
    m_port = new SerialPort();

    std::async find_port_thread(std::launch::async, [&]()
    {
        // TODO(huangchsh): 连接通知机制
        while(!m_is_find_ports_list.load())
        {
            bool ret = findPortsList();
            if(ret)
            {
                // TODO(huangchsh): 此处搜索可连接端口插入列表，发出列表信号，便于选择
                // QMap<quint16, PortInfoStru> port_info_map = getPortsList();
                // for(auto &iter : port_info_map)
                // {
                    // if(info.description() == desc)
                    // {
                    //     qDebug("Succeed to set port ");
                    //     m_is_port_found = true;

                    //     this->setPortName(info.portName());
                    //     this->setPort(info);
                    //     return true;
                    // }

                    m_is_find_ports_list.store(true);
                // }
            }
        }
    }, this);
}

SerialLink::~SerialLink()
{
    if(!getConnected())
    {
        m_port = nullptr;
    }

    disconnect();

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

void SerialLink::readBytes(const QByteArray &data)
[
    if(!m_port)
    {
        return;
    }

    m_port->readPort(data);
]

bool SerialLink::connect()
{
    if(!m_port)
    {
        return false;
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