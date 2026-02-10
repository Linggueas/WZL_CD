#include "httpmgr.h"
HttpMgr* HttpMgr::http_mgr = nullptr;
HttpMgr::HttpMgr(QObject *parent)
    : QObject{parent}
{
    //连接http请求和完成信号，信号槽机制保证队列消费
    connect(this, &HttpMgr::sig_http_finish, this, &HttpMgr::slot_http_finish);
}
HttpMgr* HttpMgr::Getinstance()
{
    if(http_mgr == nullptr)
    {
        http_mgr = new HttpMgr();
    }
    return http_mgr;
}
//发送数据
void HttpMgr::PostHttpReq(QUrl url, QJsonObject json, ReqId req_id, Modules mod)
{
    QJsonDocument json_doc(json);
    QByteArray json_data = json_doc.toJson();
    QNetworkRequest net_req(url);
    net_req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *net_rep = net_mgr.post(net_req,json_data);
    connect(net_rep,&QNetworkReply::finished,this,[this,net_rep,req_id,mod](){
        if (net_rep->error() != QNetworkReply::NoError) {
            qDebug() << net_rep->errorString();
            //发送信号通知完成
            emit sig_http_finish(req_id, "", ErrorCodes::ERR_NETWORK, mod);
            net_rep->deleteLater();
            return;
        }

        //无错误则读回请求
        QString res = net_rep->readAll();

        //发送信号通知完成
        emit sig_http_finish(req_id, res, ErrorCodes::SUCCESS, mod);
        net_rep->deleteLater();
        return;
    });
}


void HttpMgr::slot_http_finish(ReqId id, QString res, ErrorCodes err, Modules mod) {
    if (mod == Modules::REGISTERMOD) {
        //发送信号通知指定模块http响应结束
        emit sig_reg_mod_finish(id, res, err);
    }

    if (mod == Modules::RESETMOD) {
        //发送信号通知指定模块http响应结束
        emit sig_reset_mod_finish(id, res, err);
    }

    if (mod == Modules::LOGINMOD) {
        emit sig_login_mod_finish(id, res, err);
    }
}
