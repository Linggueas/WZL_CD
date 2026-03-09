#include "tcpmanger.h"
#include "log.h"
#include "QJsonDocument"
#include "QJsonObject"
#include<algorithm>
#include<QPair>
#include<QDir>
#include"dbmgr.h"
#include<QJsonArray>
#include<QFileInfo>
#include <QThread>        // 添加线程头文件
#include <QCoreApplication> // 添加应用头文件
#include <readdatathread.h>
TcpManger::TcpManger(QObject*parent):QObject(parent)
{
    tcpserver = new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any,8081);
    connect(tcpserver,&QTcpServer::newConnection,this,&TcpManger::slot_nextPending);
    qDebug() << "TcpManger thread:" << thread();
    qDebug() << "QTcpServer thread:" << tcpserver->thread();
}


TcpManger::~TcpManger()
{

}
void TcpManger::slot_nextPending()
{
    log_mutex.lock();
    Logger::getinstance()->write("成功有客户端连接");
    log_mutex.unlock();
    QTcpSocket*tcpsock = tcpserver->nextPendingConnection();
    QByteArray butter;
    _buffer.insert(tcpsock,butter);

    readDataThread *td_thread = new readDataThread(this,tcpsock);
    _rd_thread.insert(tcpsock,td_thread);
    td_thread->start();
    //td_thread->wait();
    connect(tcpsock,&QTcpSocket::disconnected,this,[tcpsock,this]{
        auto it = std::find_if(socket.begin(),socket.end(),
        [tcpsock](const auto& item){
        return item == tcpsock;
        });
        if(it!=socket.end()){
            QString key = it.key();
            QString key0 = key;
            QString message = key.append("is disconnected!");
            Logger::getinstance()->write(message);
            mutex.lock();
            socket.remove(key0);
            mutex.unlock();
            _buffer.remove(tcpsock);
            _rd_thread[tcpsock]->quit();  // 停止事件循环
			_rd_thread[tcpsock]->wait();  // 等待线程结束
            _rd_thread.remove(tcpsock);
            tcpsock->close();

            if(DBMgr::GetInstance()->
                update("user","online=0",QString("email='%1'").arg(key0)))
            {
                log_mutex.lock();
                Logger::getinstance()->write("数据库操作成功！");
                log_mutex.unlock();
            }else{
                log_mutex.lock();
                Logger::getinstance()->write("数据库操作失败！");
                log_mutex.unlock();
            }
        }
    },Qt::QueuedConnection);
}

