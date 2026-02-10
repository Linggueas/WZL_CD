#ifndef TCPMANGER_H
#define TCPMANGER_H

#include <QObject>
#include<QTcpServer>
#include<QTcpSocket>
#include<QString>
#include<global.h>
#include <QThreadPool>
typedef std::function<void(const QString&)> Function;
class TcpManger : public QObject
{
    Q_OBJECT
public:
    explicit TcpManger(QObject *parent = nullptr,
                       QString host = my_host ,QString port = my_port);
    TcpManger(const TcpManger &) = delete;
    TcpManger &operator=(const TcpManger &) = delete;
    static TcpManger*getInstance();
    void connect_to_server();
    void write(const QString &data,ReqId reqid);
private:
    QString m_host;
    QString m_port;
    uint16_t reqid;
    long long len;
    QTcpSocket*tcpsock;
    static TcpManger*tcp_manger;
    QMap<ReqId,Function>function_map;
    QByteArray _buffer;
    bool is_recv;
    QThreadPool pool;
    void initfunction_map();
signals:
    void sign_switch_main();
    void sign_refresh(QString data);
    void sign_create_file(QString data);
    void sign_down_file(QString data);
public slots:
    void slot_connect_suss();
    void slot_disconnect();
    void read();
};

#endif // TCPMANGER_H
