#include "tcpmanger.h"
#include"QMessageBox"
#include"QJsonDocument"
#include"QJsonObject"
#include"QMutex"


TcpManger::TcpManger(QObject *parent,QString host,QString port)
    : QObject{parent}
{
    initfunction_map();

    m_host = host;
    m_port = port;
    is_recv = false;
    per = 0;
    tcpsock = new QTcpSocket(this);
    connect(tcpsock,&QTcpSocket::connected,this,&TcpManger::slot_connect_suss);
    connect(tcpsock,&QTcpSocket::disconnected,this,&TcpManger::slot_disconnect);
    connect(tcpsock,&QTcpSocket::readyRead,this,&TcpManger::read);
    connect(tcpsock,&QTcpSocket::bytesWritten,this,&TcpManger::slot_bytesWritten);
}

TcpManger::~TcpManger()
{
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
    m_totalBytes = len;
    m_sentBytes = 0;
    tcpsock->write(alldata);
}

void TcpManger::slot_bytesWritten(qint64 bytes)
{
    m_sentBytes += bytes;
    int percent = static_cast<int>(100.0 * m_sentBytes / m_totalBytes);
    emit sign_down_wait_bar(percent);
}
void TcpManger::read()
{
    _buffer.append(tcpsock->readAll());
    QByteArray messageBody;
    QDataStream stream(&_buffer, QIODevice::ReadOnly);
    stream.setVersion(QDataStream::Qt_6_0);
    forever{
        if (!is_recv) {

            if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                return; // 数据不够，等待更多数据
            }

            stream >> reqid >> len;

            //将buffer 中的前四个字节移除
            _buffer = _buffer.mid(sizeof(quint16) +sizeof(long long));

            // 输出读取的数据
            qDebug() << "Message ID:" << reqid << ", Length:" << len;

        }
        // 计算当前进度（浮点，避免整数截断）
        int currentPercent = 0;
        if (len > 0) {
            currentPercent = static_cast<int>(100.0 * _buffer.size() / len);
        }

        // 限频发射：仅当百分比变化≥1%时发射
        if (qAbs(currentPercent - per) >= 1) {
            emit sign_down_wait_bar(currentPercent);
            per = currentPercent;
        }
        //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
        if (_buffer.size() < len) {
            is_recv = true;
            return;
        }

        is_recv = false;
        // 读取消息体
        messageBody = _buffer.mid(0, len);
        per = 0;
        _buffer = _buffer.mid(len);
        auto find_iter = function_map.find((ReqId)reqid);
        if (find_iter == function_map.end()) {
            qDebug() <<reqid;
            qDebug() << "not found id ";
            return;
        }

        find_iter.value()(QString(messageBody));
    }

}
TcpManger& TcpManger::getInstance()
{
    static TcpManger tcp_manger;

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

