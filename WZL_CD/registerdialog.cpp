#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "tcpmanger.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "httpmgr.h"
#include "global.h"
#include "QMessageBox"
RegisterDialog::RegisterDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    connect(HttpMgr::Getinstance(),&HttpMgr::sig_reg_mod_finish,this,&RegisterDialog::slot_reg_mod_finish);
    var = "";
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_return_button_clicked()
{
    emit sign_switch_login();
}


void RegisterDialog::on_var_pushButton_clicked()
{

    QJsonObject json_obj;
    json_obj["email"] = ui->email_ineEdit->text();
    json_obj["password"] = ui->password_lineEdit->text();
    QString m_url = "http://%1:%2/%3";
    QString _m_url =  m_url.arg(my_host).arg(my_port).arg("get_varifycode");
    qDebug()<<_m_url;
    HttpMgr::Getinstance()->PostHttpReq(
        QUrl(_m_url),json_obj,
        ReqId::ID_GET_VARIFY_CODE,
        Modules::REGISTERMOD);
    //TcpManger::getInstance()->write(json_data,ReqId:: ID_GET_VARIFY_CODE);
}
void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(id==ReqId::ID_GET_VARIFY_CODE)
    {
        QJsonDocument json_doc = QJsonDocument::fromJson(res.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto error = json_obj["error"].toInt();
        if(error != ErrorCodes::SUCCESS)
        {
            qDebug()<<"fail to get var";
        }
        var = json_obj["var"].toString();
        qDebug()<<var;
    }
    if(id == ReqId::ID_REG_USER)
    {
        if(err!=ErrorCodes::SUCCESS)
        {
            QMessageBox::critical(this,"Error","something is bad");
        }
        QJsonObject json_obj;
        json_obj["email"] = ui->email_ineEdit->text();
        json_obj["dirname"] = ui->email_ineEdit->text();
        QByteArray data = QJsonDocument(json_obj).toJson();
        TcpManger::getInstance()->connect_to_server();
        TcpManger::getInstance()->write(QString(data),ReqId::ID_CREATE_DIR);
    }
}
//注册按钮
void RegisterDialog::on_pushButton_2_clicked()
{
    QString email = ui->email_ineEdit->text();
    QString password = ui->password_lineEdit->text();
    QString _var = ui->var_lineEdit->text();
    QRegularExpression regex(
        "^[a-zA-Z0-9.!#$%&'*+"
        "/=?^_`{|}~-]+@[a-zA-Z0-9]"
        "(?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])"
        "?(?:\\.[a-zA-Z0-9](?:[a-zA-Z0-9-]{0,61}[a-zA-Z0-9])?)*$");
    if(_var!=var||password.isEmpty()||!regex.match(email).hasMatch())
    {
        QMessageBox::critical(this,"Error","something is bad");
    }
    QJsonObject json_obj;
    json_obj["email"] = email;
    json_obj["password"] = password;
    QString m_url = "http://%1:%2/%3";
    QString _m_url =
        m_url.arg(my_host).arg(my_port).arg("get_registration");
    qDebug()<<_m_url;
    HttpMgr::Getinstance()->
        PostHttpReq(
        QUrl(_m_url),json_obj,
        ReqId::ID_REG_USER,
        Modules::REGISTERMOD);

}

