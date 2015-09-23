#include <errno.h>

#include "qmodbus.h"





QModBus::QModBus() :
    QObject(NULL),

    //protected
    mb_ctx(NULL),
    strerror(""),

    //private
    connect_done(false)
{
    qRegisterMetaType<QModBus::ModBusError>("QModBus::ModBusError");
}



QModBus::~QModBus()
{

}



void QModBus::_connect()
{

    _disconnect();


    mb_ctx = create_ctx();

    if( mb_ctx == NULL )
    {
        strerror = "Can't create the libmodbus context";
        emit error(QModBus::CreateError);
        return;
    }



    if( modbus_connect(mb_ctx) == -1 )
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::ConnectionError);
        return;
    }

    connect_done = true;

    emit connected(); //good job
}



void QModBus::_disconnect()
{

    if( mb_ctx )
    {
        modbus_close(mb_ctx);
        modbus_free(mb_ctx);

        mb_ctx = NULL;
    }


    if(connect_done)
    {
        connect_done = false;

        emit disconnected();
    }
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
