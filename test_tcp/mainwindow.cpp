#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}



MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::connect_btn_clicked()
{
    if( mb.is_connected() )
        disconnect_from_dev();
    else
        connect_to_dev();
}



void MainWindow::connect_to_dev()
{

    ui->connect_button->setEnabled(false);


    //GUI param to QModBus_TCP object
    mb.IP    = ui->IPv4_line_edit->text();
    mb.port  = ui->port_line_edit->text().toInt();
    mb.set_slave(ui->slave_line_edit->text().toInt());

    //show param in Edit
    ui->log_plain_text_edit->appendPlainText(QString("###############################"));
    ui->log_plain_text_edit->appendPlainText(QString("IPv4  is: %1").arg(mb.IP));
    ui->log_plain_text_edit->appendPlainText(QString("Port  is: %1").arg(mb.port));
    ui->log_plain_text_edit->appendPlainText(QString("Slave is: %1").arg(mb.get_slave()));
    ui->log_plain_text_edit->appendPlainText(QString("-------------------------------"));


    mb.connect();
}



void MainWindow::disconnect_from_dev()
{
    ui->connect_button->setEnabled(false);
    mb.disconnect();
}
