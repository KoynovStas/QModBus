#ifndef QMODBUS_H
#define QMODBUS_H

#include <QObject>
#include <QMetaType>


#include <modbus/modbus.h>







class QModBus : public QObject
{
    Q_OBJECT
    Q_ENUMS(ModBusError)


    public:

        enum ModBusError
        {

            UnknownError = -1
        };



    public:

         virtual ~QModBus();


    protected:

        explicit QModBus();
        QModBus(const QModBus& src);
        QModBus& operator=(const QModBus&);

};



Q_DECLARE_METATYPE(QModBus::ModBusError)






#endif // QMODBUS_H
