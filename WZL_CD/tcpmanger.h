#ifndef TCPMANGER_H
#define TCPMANGER_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QString>
#include<global.h>
#include <QThread>
typedef std::function<void(const QString&)> Function;
class TcpManger : public QObject
{
    Q_OBJECT
public:
    explicit TcpManger(QObject *parent = nullptr,
                       QString host = my_host ,QString port = my_port);
    TcpManger(const TcpManger &) = delete;
    TcpManger &operator=(const TcpManger &) = delete;
    ~TcpManger();
    void connect_to_server();
    static TcpManger &getInstance();
private:
    QString m_host;
    QString m_port;
    uint16_t reqid;
    long long len;
    QTcpSocket*tcpsock;
    QMap<ReqId,Function>function_map;
    QByteArray _buffer;
    bool is_recv;
    int per;
    qint64 m_totalBytes;
    qint64 m_sentBytes;
    void initfunction_map();
signals:
    void sign_switch_main();
    void sign_refresh(QString data);
    void sign_create_file(QString data);
    void sign_down_file(QString data);
    void sign_down_wait_bar(int per);
public slots:
    void slot_connect_suss();
    void slot_disconnect();
    void read();
    void write(const QString &data,ReqId reqid);
    void slot_bytesWritten(qint64 bytes);
};

#endif // TCPMANGER_H
