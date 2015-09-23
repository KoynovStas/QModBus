#ifndef QMODBUS_H
#define QMODBUS_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QMetaType>


#include <modbus/modbus.h>







class QModBus : public QObject
{
    Q_OBJECT
    Q_ENUMS(ModBusError)


    public:

        enum ModBusError
        {
            NoConnectionError,
            CreateError,
            ConnectionError,
            SetSlaveError,
            ReadRegsError,
            WriteRegError,
            WriteRegsError,

            UnknownError = -1
        };



    public:

        virtual ~QModBus();

        bool        is_connected() { return connect_done; }
        const char *get_strerror() { return strerror; }


        void set_slave(int new_slave);
        int  get_slave() { return slave; }

        void set_response_timeout(uint32_t sec, uint32_t usec);
        void get_response_timeout(uint32_t *sec, uint32_t *usec);



    signals:

        void connected();
        void disconnected();
        void error(QModBus::ModBusError error);



    protected:

        explicit QModBus();
        QModBus(const QModBus& src);
        QModBus& operator=(const QModBus&);


        modbus_t   *mb_ctx;
        QMutex      mb_ctx_mutex;
        const char *strerror;

        int         slave;
        uint32_t    response_timeout_sec;
        uint32_t    response_timeout_usec;


        virtual modbus_t* create_ctx() = 0;

        void _connect();
        void _disconnect();

        void _set_slave(int new_slave);

        void _set_response_timeout(uint32_t sec, uint32_t usec);
        void _get_response_timeout(uint32_t *sec, uint32_t *usec);

        int _test_mb_ctx();

        int _read_regs(int addr, int num_regs, uint16_t *dest);
        int _write_reg(int addr, uint16_t value);
        int _write_regs(int addr, int num_regs, const uint16_t *data);



    private:

        bool connect_done;
};



Q_DECLARE_METATYPE(QModBus::ModBusError)





class QModBus_TCP : public QModBus
{
    Q_OBJECT


    public:

        explicit QModBus_TCP();


        QString IP;
        int     port;



    protected:

        //no copy constructor
        QModBus_TCP(const QModBus_TCP& src);
        QModBus_TCP& operator=(const QModBus_TCP&);


        virtual modbus_t* create_ctx();
};





class QModBus_RTU : public QModBus
{
    Q_OBJECT


    public:

        explicit QModBus_RTU();


        QString device;
        int     baudrate;
        char    parity;
        int     data_bit;
        int     stop_bit;



    protected:

        //no copy constructor
        QModBus_RTU(const QModBus_RTU& src);
        QModBus_RTU& operator=(const QModBus_RTU&);


        virtual modbus_t* create_ctx();
};





#endif // QMODBUS_H
