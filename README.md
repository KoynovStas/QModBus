# QModBus is wrapper over libmodbus for Qt


### Note
This class is **deprecated**. It was developed when Qt did not support the work with the ModBus protocol. 
Now you should use the standard Qt interface to work with ModBus(Since Qt 5.8):[Qt Modbus](https://doc.qt.io/qt-5/qtmodbus-backends.html).

### Examples from Qt Documentation
* [Modbus Slave example](https://doc.qt.io/qt-5/qtserialbus-modbus-slave-example.html)
* [Modbus Master example](https://doc.qt.io/qt-5/qtserialbus-modbus-master-example.html)



## Description
QModBus is abstract C++ class for Qt. QModBus is wrapper over libmodbus for Qt. 
From this abstract class inherited two specific classes: **QModBus_TCP** and **QModBus_RTU**. 
This class provides the opportunity to work with the library [(libmodbus ver 3.1.2)](http://www.libmodbus.org) in not blocking mode.


**The class has the following public methods:**
```C++
bool        is_connected() { return connect_done; }
const char *get_strerror() { return strerror; }


void set_slave(int new_slave);
int  get_slave() { return slave; }

void set_response_timeout(uint32_t sec, uint32_t usec);
void get_response_timeout(uint32_t *sec, uint32_t *usec);
```


**The class has the following public signals:**
```C++
signals:

    void connected();
    void disconnected();
    void error(QModBus::ModBusError error);

    void response_to_read_regs(int status);
    void response_to_write_reg(int status);
    void response_to_write_regs(int status);
```


**The class has the following public slots:**
```C++
public slots:

    virtual void connect();
    virtual void disconnect();

    virtual void read_regs(int addr, int num_regs, uint16_t *dest);
    virtual void write_reg(int addr, uint16_t value);
    virtual void write_regs(int addr, int num_regs, const uint16_t *data);
```


**The class has the following public enums:**
```C++
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
```

More details see: **[qmodbus.h](./src/include/qmodbus.h)**


***
## Usage

**To start working, perform the following steps:**

1. You need to include **[qmodbus.h](./src/include/qmodbus.h)** file in your **.cpp** file.
2. And add file **[qmodbus.cpp](./src/qmodbus.cpp)** to list of source files to compile (to qmake project file). (see an example)


***
## Examples

1. **[test_tcp](./test_tcp)** - how to work with the class QModBus_TCP

![Test_TCP](./screenshots/Test_TCP.png)



2. **[test_rtu](./test_rtu)** - how to work with the class QModBus_RTU

![Test_RTU](./screenshots/Test_RTU.png)


## Note:
> In OS Linux Device for test_rtu is: **/dev/tty***  default is: **/dev/ttyUSB0**

> In OS Windows Device for test_rtu is: **\\\\.\COM***



## Build tests

```console
qmake
make
```



## License

[BSD-3-Clause](./LICENSE).



## Copyright
Copyright (C) 2015 Koynov Stas - skojnov@yandex.ru
