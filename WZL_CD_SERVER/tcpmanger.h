#ifndef TCPMANGER_H
#define TCPMANGER_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include <functional>
#include<global.h>
#include <readdatathread.h>


typedef std::function<void(QTcpSocket*,const QString&)> Function;
class TcpManger : public QObject
{
    Q_OBJECT
public:
    TcpManger(QObject*parent);
    ~TcpManger();
    QTcpServer*tcpserver;
private:
    QMap<QString,QTcpSocket*>socket;
    QMap<ReqId,Function>function_map;
    void initfunction_map();
    void write(const QString &data,ReqId reqid,QTcpSocket *tcpsock);
    QString traverseFolder(const QString &folderPath);
    QMap<QTcpSocket*,QByteArray> _buffer;
    QMap<QTcpSocket*,readDataThread*>_rd_thread;
    bool _b_recv_pending;
    quint16 _message_id;
    long long _message_len;


    static const int MAX_THREAD_COUNT = 3;

    QAtomicInteger<int> m_activeConnections;

public slots:
    void slot_nextPending();
signals:
    void sign_nextPending();

};

#endif // TCPMANGER_H
