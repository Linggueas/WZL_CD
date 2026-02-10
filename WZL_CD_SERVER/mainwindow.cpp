#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "log.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcp_manger = new TcpManger(this);
    connect(Logger::getinstance(),&Logger::sign_write_log,this,&MainWindow::sign_wirte_log);
    DBMgr::GetInstance();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::sign_wirte_log(const QString &__log)
{
    QString ___log = __log;
    //___log.append("\n");
    ui->textEdit->append(___log);
}
