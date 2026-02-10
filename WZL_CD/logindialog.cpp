#include "logindialog.h"
#include "ui_logindialog.h"
#include"httpmgr.h"
#include"QMessageBox"
#include"tcpmanger.h"
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(HttpMgr::Getinstance(),
            &HttpMgr::sig_login_mod_finish,
            this,&LoginDialog::slot_login_mod_finish);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_regis_Button_clicked()
{
    emit sign_switch_register();
}

//登录
void LoginDialog::on_login_Button_clicked()
{
    //TcpManger::getInstance()->connect_to_server();
    if(ui->email_lineEdit->text().isEmpty()||ui->password_lineEdit->text().isEmpty())
    {
        QMessageBox::critical(this,"Error","something is bad");
    }
    QJsonObject json_obj;
    json_obj["email"] = ui->email_lineEdit->text();
    json_obj["password"] = ui->password_lineEdit->text();
    QString m_url = "http://%1:%2/%3";
    QString _m_url =
        m_url.arg(my_host).arg(my_port).arg("get_login");
    HttpMgr::Getinstance()->PostHttpReq(
        QUrl(_m_url),json_obj,
        ReqId::ID_LOGIN_USER,
        Modules::LOGINMOD);

}

//处理http返回
void LoginDialog::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(id==ReqId::ID_LOGIN_USER)
    {
        my_email = ui->email_lineEdit->text();
        qDebug()<<"success!!!";
        TcpManger::getInstance()->connect_to_server();
        QJsonObject json_obj;
        json_obj["email"] = my_email;
        QByteArray by_data = QJsonDocument(json_obj).toJson();
        TcpManger::getInstance()->write
            (QString(by_data),ReqId::ID_LOGIN_USER);
        connect(TcpManger::getInstance(),&TcpManger::sign_switch_main,this,&LoginDialog::slot_switch_main);
    }
}


void LoginDialog::slot_switch_main()
{
    emit sign_switch_main();
}
