#include "mainwindow.h"
#include "ui_mainwindow.h"





MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    QObject::connect(ui->connect_button, SIGNAL(clicked()),
                     this,  SLOT(connect_btn_clicked()));



    //read button
    QObject::connect(ui->rd_button, SIGNAL(clicked()),
                     this, SLOT(read_regs()));


    QObject::connect(&mb,  SIGNAL(response_to_read_regs(int)),
                     this, SLOT(response_to_read_regs(int)));



    //write button
    QObject::connect(ui->wr_button, SIGNAL(clicked()),
                     this, SLOT(write_reg()));


    QObject::connect(&mb,  SIGNAL(response_to_write_reg(int)),
                     this, SLOT(response_to_write_reg(int)));
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



void MainWindow::read_regs()
{

    int addr  = ui->rd_reg_line_edit->text().toInt();
    int count = ui->rd_count_line_edit->text().toInt();

    if( count > MODBUS_MAX_READ_REGISTERS)
    {
        ui->log_plain_text_edit->appendPlainText(QString("Error count reg is: %1").arg(count));
        ui->log_plain_text_edit->insertPlainText(QString("Max count reg is: %1").arg(MODBUS_MAX_READ_REGISTERS));

        return;
    }

    if( count <= 0)
    {
        ui->log_plain_text_edit->appendPlainText(QString("Error count reg is: %1").arg(count));
        return;
    }


    ui->log_plain_text_edit->appendPlainText(QString("Read Start register: %1 ").arg(addr));
    ui->log_plain_text_edit->insertPlainText(QString("Count: %1").arg(count));

    ui->rd_button->setEnabled(false);

    mb.read_regs(addr, count, rd_buf);
}



void MainWindow::write_reg()
{
    int addr  = ui->wr_reg_line_edit->text().toInt();
    int value = ui->wr_val_line_edit->text().toInt();


    ui->log_plain_text_edit->appendPlainText(QString("Write to register: %1 ").arg(addr));
    ui->log_plain_text_edit->insertPlainText(QString("value: %1").arg(value));

    ui->wr_button->setEnabled(false);

    mb.write_reg(addr, value);
}



void MainWindow::response_to_read_regs(int status)
{
    int i;

    ui->rd_button->setEnabled(true);

    if( status <= 0 )
        return;


    ui->log_plain_text_edit->appendPlainText(QString("Reading regs: %1").arg(status));

    for(i = 0; i < status; i++)
    {
        ui->log_plain_text_edit->appendPlainText(QString("reg[%1] == %2").arg(i).arg(rd_buf[i]));
    }
}



void MainWindow::response_to_write_reg(int status)
{
    ui->wr_button->setEnabled(true);

    if( status == 1 )
    {
        ui->log_plain_text_edit->appendPlainText(QString("Write done"));
    }
}
