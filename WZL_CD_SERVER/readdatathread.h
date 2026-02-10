#ifndef READDATATHREAD_H
#define READDATATHREAD_H

#include <QObject>
#include <QThread>
#include <QTcpSocket>
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
    void run()override;
signals:
    void taskFinished(QByteArray messageBody,quint16 _message_id);
};

#endif // READDATATHREAD_H
