#include "mainwindow.h"
#include <QApplication>
#include<QFile>
#include<QMessageBox>
#include<global.h>
#include<QJsonObject>
#include<QJsonDocument>
#include<QDebug>
#include<tcpmanger.h>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //TcpManger::getInstance();
    MainWindow w;
    QString data;
    QFile file(":/new/prefix1/config.json");
    if(file.open(QIODevice::ReadOnly)){
        QByteArray badata = file.readAll();
        data = badata.toStdString().c_str();
    }else{
        qDebug()<<"no this file";
        return 0;
        //QMessageBox::warning(this,"警告","没有这个文件");
    }
    QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonObject json_obj = json_doc.object();
    my_host = json_obj["host"].toString();
    my_port = json_obj["port"].toString();
    qDebug()<<my_host<<"   "<<my_port;
    w.show();
    //
    return a.exec();
}
