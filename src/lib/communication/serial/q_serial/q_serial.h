#pragma once

#include <QStringList>
#include <QMutexLocker>
#include <QSerialPort>
#include <QSerialPortInfo>


namespace port_lib
{
    struct PortInfoStru
    {
        explicit PortInfoStru()
        {
            vendor_identifier = 0;
            product_identifier = 0;
            name = "";
            system_location = "";
            description = "";
            manufacturer = "";
            serial_number = "";
        };

        quint16 vendor_identifier;
        quint16 product_identifier;

        QString name;
        QString system_location;
        QString description;
        QString manufacturer;
        QString serial_number;
    };
    // TODO(huangchsh): __WEAK 抽象为接口

    class SerialPort : public QSerialPort
    {
        public:
            using QDataBits = DataBits;
            using QParity = Parity;
            using QStopBits = StopBits;
            using QFlowControl = FlowControl;
            using QPinoutSignal = PinoutSignal;
            using QSerialPortError = SerialPortError;

        public:
            explicit SerialPort() = default;
            explicit SerialPort(const QString &port_name,
                                qint32 baud_rate = 115200,
                                QDataBits data_bits = QDataBits::Data8,
                                QParity parity = QParity::NoParity,
                                QStopBits stop_bits = QStopBits::OneStop,
                                QFlowControl flow_control = QFlowControl::NoFlowControl);
            explicit SerialPort(const QString &port_name, qint32 baud_rate = 115200);
            explicit SerialPort(const QString &port_name);
            ~SerialPort();

            bool openPort();
            void closePort();
            qint64 readPort(QByteArray &buffer);
            qint64 writePort(const QByteArray &buffer);

            static QList<PortInfoStru> findPortsList();

        private:
            SerialPort(const SerialPort&);
            SerialPort& operator=(const SerialPort&);

            QMutex m_write_mutex;
            QMutex m_read_mutex;
    };
}