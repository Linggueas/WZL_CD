#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmanger.h"
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    log_dlg = new LoginDialog(this);
    log_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    resize(log_dlg->size());
    setCentralWidget(log_dlg);
    connect(log_dlg,&LoginDialog::sign_switch_register,this,&MainWindow::slot_switch_register);
    connect(log_dlg,&LoginDialog::sign_switch_main,this,&MainWindow::slot_switch_main);
    m_tcpThread = new QThread();
    connect(m_tcpThread, &QThread::started,[]() {
        qDebug() << "TcpManger线程已启动，线程ID:" << QThread::currentThreadId();
    });
    // 创建TCP管理线程

    // 获取TcpManger单例

    // 将TcpManger移动到线程
    //tcpManager->moveToThread(m_tcpThread);

    /*connect(m_tcpThread, &QThread::started, tcpManager, []() {
        qDebug() << "TcpManger线程已启动，线程ID:" << QThread::currentThreadId();
    });*/

    // 启动线程
    //m_tcpThread->start();

}


MainWindow::~MainWindow()
{
    // 停止线程
    if (m_tcpThread && m_tcpThread->isRunning()) {
        m_tcpThread->quit();
        m_tcpThread->wait();
        delete m_tcpThread;
    }
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(this,"提醒","开始连接");
    //TcpManger*tcp = new TcpManger(this,my_host,my_port);

}
void MainWindow::slot_switch_register()
{
    regis_dlg = new RegisterDialog(this);
    regis_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);

    resize(regis_dlg->size());
    setCentralWidget(regis_dlg);
    connect(regis_dlg,&RegisterDialog::sign_switch_login,this,&MainWindow::slot_switch_login);

}
void MainWindow::slot_switch_login()
{
    log_dlg = new LoginDialog(this);
    log_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    resize(log_dlg->size());
    setCentralWidget(log_dlg);
    connect(log_dlg,&LoginDialog::sign_switch_register,this,&MainWindow::slot_switch_register);
    connect(log_dlg,&LoginDialog::sign_switch_main,this,&MainWindow::slot_switch_main);
}

void MainWindow::slot_switch_main()
{
    main_dlg = new MainDialog(this);
    log_dlg->hide();
    main_dlg->setWindowFlags(Qt::CustomizeWindowHint | Qt::FramelessWindowHint);
    resize(main_dlg->size());
    setCentralWidget(main_dlg);

}




