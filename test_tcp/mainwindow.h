#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qmodbus.h"




namespace Ui
{
    class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

    public:

        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();



    public slots:

        void connect_btn_clicked();
        void connect_to_dev();
        void disconnect_from_dev();



    private:

        Ui::MainWindow *ui;

        QModBus_TCP  mb;

        uint16_t  rd_buf[MODBUS_MAX_READ_REGISTERS];
};



#endif // MAINWINDOW_H
