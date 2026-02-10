#include "tcpmanger.h"
#include"QMessageBox"
#include"QJsonDocument"
#include"QJsonObject"
#include"QMutex"
TcpManger *TcpManger::tcp_manger = nullptr;


TcpManger::TcpManger(QObject *parent,QString host,QString port)
    : QObject{parent}
{
    initfunction_map();
    pool.setMaxThreadCount(4);
    m_host = host;
    m_port = port;
    is_recv = false;
    tcpsock = new QTcpSocket(this);
    connect(tcpsock,&QTcpSocket::connected,this,&TcpManger::slot_connect_suss);
    connect(tcpsock,&QTcpSocket::disconnected,this,&TcpManger::slot_disconnect);
    connect(tcpsock,&QTcpSocket::readyRead,this,&TcpManger::read);
}

void TcpManger::connect_to_server()
{
    if(tcpsock->state()==QTcpSocket::ConnectedState)
    {
        return;
    }
    tcpsock->connectToHost(QHostAddress(m_host),my_port.toUInt()+1);
    qDebug()<<"-----------------";
    qDebug()<<my_host;
    qDebug()<<my_port.toUInt()+1;
}
void TcpManger::slot_connect_suss()
{
    //QMessageBox::information(nullptr,"a","a");
    qDebug()<<"connect success!";
}
void TcpManger::slot_disconnect()
{
    qDebug()<<"connect fail!";
}

void TcpManger::write(const QString &data,ReqId reqid)
{
    QByteArray alldata;
    uint16_t id = reqid;
    long long len = 0; // 初始化为0
    QByteArray main_data = data.toUtf8();

    len = main_data.size(); // 设置 len 为有效载荷长度
    qDebug()<<len;
    QDataStream data_stream(&alldata, QIODevice::WriteOnly);
    data_stream << id << len;
    qDebug()<<alldata;
    alldata.append(main_data);
    qDebug()<<alldata;
    tcpsock->write(alldata);
}
void TcpManger::read()
{
    _buffer.append(tcpsock->readAll());
    qDebug()<<_buffer;
    if(!is_recv)
    {
        if (_buffer.size() < static_cast<int>(sizeof(quint16) + sizeof(long long))) {
            return; // 数据不够，等待更多数据
        }
        QDataStream data_stream(&_buffer,QIODevice::ReadOnly);
        data_stream>>reqid;
        data_stream>>len;
        _buffer = _buffer.mid(sizeof(quint16) +sizeof(long long));
    }
    if(_buffer.size()<len)
    {
        is_recv = true;
        return;
    }
    is_recv = false;
    QByteArray main_data = _buffer.mid(0, len);
    qDebug()<<QString(_buffer);
    _buffer.clear();
    function_map[(ReqId)reqid](QString(main_data));

}
TcpManger* TcpManger::getInstance()
{
    if (tcp_manger == nullptr) {
        tcp_manger = new TcpManger();
    }
    return tcp_manger;
}
void TcpManger::initfunction_map()
{
    function_map.insert(ReqId::ID_LOGIN_USER,[this](QString data){
        qDebug()<<data;
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto error = json_obj["error"];
        if(error != ErrorCodes::SUCCESS)
        {
            qDebug()<<"登录失败";
            return;
        }
        emit sign_switch_main();
    });
    function_map.insert(ReqId::ID_CREATE_DIR,[this](QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto error = json_obj["error"];
        if(error == ErrorCodes::SUCCESS)
        {
            qDebug()<<"登录失败";
            return;
        }
    });
    function_map.insert(ReqId::ID_REFRESH,[this](QString data){
        emit sign_refresh(data);
    });
    function_map.insert(ReqId::ID_CREATE_FILE,[this](QString data){
        emit sign_create_file(data);
    });
    function_map.insert(ReqId::ID_DOWN_FILE,[this](QString data){
        emit sign_down_file(data);
    });
}

