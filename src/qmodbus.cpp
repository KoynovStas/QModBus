#include "qmodbus.h"






QModBus::QModBus() :
    QObject(NULL)

{
    qRegisterMetaType<QModBus::ModBusError>("QModBus::ModBusError");
}



QModBus::~QModBus()
{

}





//---------------------- QModBus_TCP ----------------------





QModBus_TCP::QModBus_TCP() :
    QModBus(),

    //public
    IP("127.0.0.1"),
    port(MODBUS_TCP_DEFAULT_PORT)
{

}



modbus_t* QModBus_TCP::create_ctx()
{
    return modbus_new_tcp(IP.toStdString().c_str(), port);
}





//---------------------- QModBus_RTU ----------------------





QModBus_RTU::QModBus_RTU() :
    QModBus(),

    //public
    device("/dev/ttyUSB0"),
    baudrate(115200),
    parity('N'),
    data_bit(8),
    stop_bit(1)
{

}



modbus_t* QModBus_RTU::create_ctx()
{
    return modbus_new_rtu(device.toStdString().c_str(), baudrate, parity, data_bit, stop_bit);
}
