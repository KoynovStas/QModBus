#include <errno.h>
#include <QMutexLocker>

#include "qmodbus.h"





QModBus::QModBus() :
    QObject(NULL),

    //protected
    mb_ctx(NULL),
    strerror(""),
    slave(0),
    response_timeout_sec(5),
    response_timeout_usec(0),


    //private
    connect_done(false)
{
    qRegisterMetaType<QModBus::ModBusError>("QModBus::ModBusError");

    this->moveToThread(&thread);

    thread.start();
}



QModBus::~QModBus()
{
    thread.quit();
    thread.wait();

    _disconnect();
}



void QModBus::set_slave(int new_slave)
{
    QMutexLocker locker(&mb_ctx_mutex);

    _set_slave(new_slave);
}



void QModBus::set_response_timeout(uint32_t sec, uint32_t usec)
{
    QMutexLocker locker(&mb_ctx_mutex);

    _set_response_timeout(sec, usec);
}



void QModBus::get_response_timeout(uint32_t *sec, uint32_t *usec)
{
    QMutexLocker locker(&mb_ctx_mutex);

    _get_response_timeout(sec, usec);
}



int QModBus::read_regs(int addr, int num_regs, uint16_t *dest)
{
    QMutexLocker locker(&mb_ctx_mutex);

    return _read_regs(addr, num_regs, dest);
}



int QModBus::write_reg(int addr, uint16_t value)
{
    QMutexLocker locker(&mb_ctx_mutex);

    return _write_reg(addr, value);
}



int QModBus::write_regs(int addr, int num_regs, const uint16_t *data)
{
    QMutexLocker locker(&mb_ctx_mutex);

    return _write_regs(addr, num_regs, data);
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


    //set timeout for modbus_connect function
    _set_response_timeout(response_timeout_sec, response_timeout_usec);

    if( modbus_connect(mb_ctx) == -1 )
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::ConnectionError);
        return;
    }

    connect_done = true;


    _set_slave(slave);
    _set_response_timeout(response_timeout_sec, response_timeout_usec);


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



void QModBus::_set_slave(int new_slave)
{
    slave = new_slave;


    if( !is_connected() )
        return;


    if( modbus_set_slave(mb_ctx, slave) == -1 )
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::SetSlaveError);
    }
}



void QModBus::_set_response_timeout(uint32_t sec, uint32_t usec)
{

    response_timeout_sec  = sec;
    response_timeout_usec = usec;


    if( is_connected() )
        modbus_set_response_timeout(mb_ctx, sec, usec);
}



void QModBus::_get_response_timeout(uint32_t *sec, uint32_t *usec)
{

    if( is_connected() )
        modbus_get_response_timeout(mb_ctx, sec, usec);
    else
    {
       *sec  = response_timeout_sec;
       *usec = response_timeout_usec;
    }
}



int QModBus::_test_mb_ctx()
{

    if( mb_ctx == NULL )
    {
        strerror = "No Connection";
        emit error(QModBus::NoConnectionError);
        return -1;
    }


    return 0; //good job  mb_ctx is valid
}



int QModBus::_read_regs(int addr, int num_regs, uint16_t *dest)
{

    if( _test_mb_ctx() != 0 )
        return -1;


    int ret = modbus_read_registers(mb_ctx, addr, num_regs, dest);
    if(ret == -1)
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::ReadRegsError);
        return -1;
    }


    return ret; // return the number of read registers
}



int QModBus::_write_reg(int addr, uint16_t value)
{

    if( _test_mb_ctx() != 0 )
        return -1;


    int ret = modbus_write_register(mb_ctx, addr, value);
    if( ret == -1 )
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::WriteRegError);
        return -1;
    }


    return ret; // return 1 if successful
}



int QModBus::_write_regs(int addr, int num_regs, const uint16_t *data)
{

    if( _test_mb_ctx() != 0 )
        return -1;


    int ret = modbus_write_registers(mb_ctx, addr, num_regs, data);
    if( ret == -1 )
    {
        strerror = modbus_strerror(errno);
        emit error(QModBus::WriteRegsError);
        return -1;
    }


    return ret; // return the number of written registers
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
