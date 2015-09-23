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


    private:

        Ui::MainWindow *ui;

        QModBus_RTU  mb;
};



#endif // MAINWINDOW_H
