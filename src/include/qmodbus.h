/*
 * qmodbus.h
 *
 *
 * version 1.0
 *
 *
 * Copyright (c) 2015, Koynov Stas - skojnov@yandex.ru
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1 Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  2 Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  3 Neither the name of the <organization> nor the
 *    names of its contributors may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef QMODBUS_H
#define QMODBUS_H

#include <QObject>
#include <QString>
#include <QMutex>
#include <QMetaType>
#include <QEvent>


#include <modbus/modbus.h>
#include "qexecthread.h"






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

        virtual bool event(QEvent *); //for fast delete

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

        void response_to_read_regs(int status);
        void response_to_write_reg(int status);
        void response_to_write_regs(int status);


        //signals for inner use
        void run_connect();
        void run_disconnect();
        void run_read_regs(int addr, int num_regs, uint16_t *dest);
        void run_write_reg(int addr, uint16_t value);
        void run_write_regs(int addr, int num_regs, const uint16_t *data);



    public slots:

        virtual void connect();
        virtual void disconnect();

        virtual void read_regs(int addr, int num_regs, uint16_t *dest);
        virtual void write_reg(int addr, uint16_t value);
        virtual void write_regs(int addr, int num_regs, const uint16_t *data);



    protected slots:

        void lock_connect();
        void lock_disconnect();

        void lock_read_regs(int addr, int num_regs, uint16_t *dest);
        void lock_write_reg(int addr, uint16_t value);
        void lock_write_regs(int addr, int num_regs, const uint16_t *data);



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

        bool         connect_done;
        QExecThread  thread;
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
