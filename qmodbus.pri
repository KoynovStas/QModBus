TEMPLATE = app


QT  += core
QT  += gui
QT  += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets



CONFIG += qt
CONFIG += debug_and_release  build_all



CONFIG(debug, debug|release) {
    TARGETDIR = debug
    DESTDIR   = debug
}



CONFIG(release, debug|release) {
    TARGETDIR = release
    DESTDIR   = release
}



SOURCES_DIR = $$PWD/src
INCLUDE_DIR = $$PWD/src/include


DEPENDPATH  += . $$SOURCES_DIR $$INCLUDE_DIR
INCLUDEPATH += . $$SOURCES_DIR $$INCLUDE_DIR



unix: {


       contains(QT_ARCH, x86_64) {
           LIBMODBUS_DIR = $$PWD/libmodbus/unix_x86_64
       }else {
           LIBMODBUS_DIR = $$PWD/libmodbus/unix_x86
       }

       LIBS += -L$$LIBMODBUS_DIR -Wl,-rpath=. -lmodbus

       copy_lib.commands =  cp    $$LIBMODBUS_DIR/libmodbus.so.5.1.0 $$TARGETDIR &&
       copy_lib.commands += cp -d $$LIBMODBUS_DIR/libmodbus.so.5     $$TARGETDIR &&
       copy_lib.commands += cp -d $$LIBMODBUS_DIR/libmodbus.so       $$TARGETDIR
}



win32: {

       contains(QT_ARCH, x86_64) {
           LIBMODBUS_DIR = $$PWD/libmodbus/win_x86_64
       }else {
           LIBMODBUS_DIR = $$PWD/libmodbus/win_x86
       }


       LIBMODBUS_DIR ~= s,/,\\,g

       LIBS += -L$$LIBMODBUS_DIR -Wl,-rpath=. -lmodbus

       copy_lib.commands =  $$quote($$QMAKE_COPY  $$LIBMODBUS_DIR\\libmodbus-5.dll  .\\$$TARGETDIR)
}



QMAKE_EXTRA_TARGETS += copy_lib

PRE_TARGETDEPS += copy_lib



# Input
SOURCES  += main.cpp \
            mainwindow.cpp \
            $$SOURCES_DIR/qmodbus.cpp



HEADERS  += mainwindow.h \
            $$INCLUDE_DIR/modbus/modbus-rtu.h \
            $$INCLUDE_DIR/modbus/modbus-tcp.h \
            $$INCLUDE_DIR/modbus/modbus-version.h \
            $$INCLUDE_DIR/modbus/modbus.h \
            $$INCLUDE_DIR/qmodbus.h \
            $$INCLUDE_DIR/qexecthread.h \
            $$INCLUDE_DIR/asyncdeltask.h



FORMS    += mainwindow.ui
