#include "qmodbus.h"






QModBus::QModBus() :
    QObject(NULL)

{
    qRegisterMetaType<QModBus::ModBusError>("QModBus::ModBusError");
}



QModBus::~QModBus()
{

}
