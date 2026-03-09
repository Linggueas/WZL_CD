#ifndef READDATATHREAD_H
#define READDATATHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
#include <global.h>
#include "log.h"
#include "QJsonDocument"
#include "QJsonObject"
#include<algorithm>
#include<QPair>
#include<QDir>
#include"dbmgr.h"
#include<QJsonArray>
#include<QFileInfo>
typedef std::function<void(QTcpSocket*,const QString&)> Function;
class readDataThread :public QThread
{
    Q_OBJECT
public:
    explicit readDataThread(QObject *parent = nullptr,QTcpSocket*tcpsock = nullptr);
private:
    QTcpSocket*tcpsock;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    long long _message_len;
    int per;
    QMap<ReqId,Function>function_map;
    void run()override;
    void initfunction_map();
    void write(const QString &data, ReqId reqid, QTcpSocket *tcpsock);
    QString traverseFolder(const QString &folderPath);
signals:
    void taskFinished(QByteArray messageBody,quint16 _message_id);
};

#endif // READDATATHREAD_H
