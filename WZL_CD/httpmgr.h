#ifndef HTTPMGR_H
#define HTTPMGR_H

#include <QObject>
#include<QNetworkAccessManager>
#include<QNetworkReply>
#include<global.h>
#include<QJsonDocument>
#include<QJsonObject>
class HttpMgr : public QObject
{
    Q_OBJECT
public:
    explicit HttpMgr(QObject *parent = nullptr);
    static HttpMgr*Getinstance();
    void PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod);
private:
    static HttpMgr*http_mgr;
    QNetworkAccessManager net_mgr;
private slots:
    void slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

signals:
    void sig_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod);

    //注册模块http相关请求完成发送此信号
    void sig_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_reset_mod_finish(ReqId id, QString res, ErrorCodes err);
    void sig_login_mod_finish(ReqId id, QString res, ErrorCodes err);
};

#endif // HTTPMGR_H
