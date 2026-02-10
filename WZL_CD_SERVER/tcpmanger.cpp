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
    initfunction_map();
    connect(tcpserver,&QTcpServer::newConnection,this,&TcpManger::slot_nextPending);
    qDebug() << "TcpManger thread:" << thread();
    qDebug() << "QTcpServer thread:" << tcpserver->thread();
}


TcpManger::~TcpManger()
{

}
void TcpManger::slot_nextPending()
{
    Logger::getinstance()->write("成功有客户端连接");
    QTcpSocket*tcpsock = tcpserver->nextPendingConnection();
    QByteArray butter;
    _buffer.insert(tcpsock,butter);

    readDataThread *td_thread = new readDataThread(this,tcpsock);
    _rd_thread.insert(tcpsock,td_thread);
    td_thread->start();
    //td_thread->wait();
    connect(td_thread,&readDataThread::taskFinished,this,[this,tcpsock](QByteArray messageBody,quint16 _message_id){
        //tcpsock->moveToThread(QThread::currentThread());
        auto find_iter = function_map.find((ReqId)_message_id);
        if (find_iter == function_map.end()) {
            qDebug() <<_message_id;
            qDebug() << "not found id ";
            return;
        }

        find_iter.value()(tcpsock,messageBody);

    }, Qt::QueuedConnection);
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
            socket.remove(key0);
            _buffer.remove(tcpsock);
            _rd_thread[tcpsock]->quit();  // 停止事件循环
			_rd_thread[tcpsock]->wait();  // 等待线程结束
            _rd_thread.remove(tcpsock);
            tcpsock->close();
            if(DBMgr::GetInstance()->
                update("user","online=0",QString("email='%1'").arg(key0)))
            {
                Logger::getinstance()->write("数据库操作成功！");
            }else{
                Logger::getinstance()->write("数据库操作失败！");
            }
        }
    },Qt::QueuedConnection);
}

//接口初始化
void TcpManger::initfunction_map()
{

    //登录
    function_map.insert(ReqId::ID_LOGIN_USER,[this](QTcpSocket *tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        socket.insert(email,tcpsock);
        QJsonObject jsonobj;
        jsonobj["error"] = ErrorCodes::SUCCESS;
        QByteArray Bydata = QJsonDocument(jsonobj).toJson();
        write(QString(Bydata),ReqId::ID_LOGIN_USER,tcpsock);
    });
    //创建文件夹
    function_map.insert(ReqId::ID_CREATE_DIR,[this](QTcpSocket *tcpsock,QString data){

        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        auto dirname = json_doc["dirname"].toString();
        Logger::getinstance()->
            write(QString("%1创建文件夹%2").arg(email).arg(dirname));
        QDir dir;
        bool error = dir.mkdir(dirname);

    });
    //刷新
    function_map.insert(ID_REFRESH,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        QString json_data =  traverseFolder(email);
        Logger::getinstance()->
            write(QString("%1请求刷新").arg(email));
        QJsonDocument json_doc0 = QJsonDocument::fromJson(json_data.toUtf8());
        QJsonObject json_obj0 = json_doc0.object();
        QJsonArray json_arr;
        json_arr.append(json_obj0);
        write(QString(QJsonDocument(json_arr).toJson()),ReqId::ID_REFRESH,tcpsock);
        //etAllPathsRecursive(email);
    });
    //删除
    function_map.insert(ID_REMOVE,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        auto dir = json_obj["dir"].toString();
        auto type = json_obj["type"].toString();
        Logger::getinstance()->write(QString("%1 删除文件/文件夹：%2").arg(email).arg(dir));

        if(type == "m_file"){
            QDir _dir;
            _dir.remove(dir);
        }else if(type == "m_dir")
        {
            QDir _dir(dir);
            _dir.removeRecursively();
        }
    });
    //创建文件
    function_map.insert(ReqId::ID_CREATE_FILE,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        QString file_data = json_obj["file_data"].toString();
        QByteArray fileData = QByteArray::fromBase64(file_data.toUtf8());
        auto dir = json_obj["dir"].toString();
        auto file_name = json_obj["file_name"].toString();
        Logger::getinstance()->write(QString("%1 添加文件/文件夹：%2/%3").arg(email).arg(dir).arg(file_name));
        QString filePath = "%1/%2";
        filePath =  filePath.arg(dir).arg(file_name);
        QFileInfo fileInfo(filePath);
        QDir _dir = fileInfo.dir();

        if (!_dir.exists()) {
            if (!_dir.mkpath(".")) {
                qDebug() << "Failed to create directory:" << _dir.path();
            }
        }
        QFile file(filePath);
        if (!file.open(QIODevice::WriteOnly)) {
            qDebug() << "Failed to create file:" << file.errorString();
        }
        file.write(fileData);
        file.close();
        QJsonObject json_obj0;
        json_obj0["error"] = ErrorCodes::SUCCESS;
        write(QString(QJsonDocument(json_obj0).toJson()),ReqId::ID_CREATE_FILE,tcpsock);

    });
    //下载文件
    function_map.insert(ReqId::ID_DOWN_FILE,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        QString dir = json_obj["dir"].toString();
        QStringList dirlist = dir.split("/");
        Logger::getinstance()->
            write(QString("%1下载文件%2").arg(email).arg(dirlist[dirlist.size()-1]));
        QFile file(dir);
        file.open(QIODevice::ReadOnly);
        QByteArray file_data = file.readAll();
        QStringList file_list = dir.split("/");
        QString file_name = file_list[file_list.size()-1];
        qDebug()<<file_name;
        QByteArray base64Data = file_data.toBase64();
        QJsonObject json_obj0;
        json_obj0["file_name"] = file_name;
        json_obj0["file_data"] = QString::fromUtf8(base64Data);
        write(QString(QJsonDocument(json_obj0).toJson()),ReqId::ID_DOWN_FILE,tcpsock);

    });
    function_map.insert(ReqId::ID_NEW_NAME,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        QString old_name = json_obj["old_dir"].toString();
        QString new_name  = json_obj["new_name"].toString();
        QStringList old_name_list = old_name.split("/");
        old_name_list[old_name_list.size()-1] = new_name;
        new_name = old_name_list.join("/");
        qDebug()<<old_name;
        qDebug()<<new_name;
        QDir dir;
        dir.rename(old_name,new_name);
    });
}


//
void TcpManger::write(const QString &data,ReqId reqid,QTcpSocket *tcpsock)
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
//
QString TcpManger::traverseFolder(const QString &folderPath) {
    QJsonObject node;
    QDir dir(folderPath);
    QFileInfo fileInfo(folderPath);

    // 设置节点名称（使用文件夹名而不是完整路径）
    node["name"] = fileInfo.fileName();

    if (fileInfo.isDir()) {
        node["type"] = "dir";

        // 获取文件夹内所有条目（排除.和..）
        QStringList entries = dir.entryList(QDir::NoDotAndDotDot | QDir::AllEntries);
        if (!entries.isEmpty()) {
            QJsonArray sonArray;

            foreach (const QString &entry, entries) {
                QString fullPath = dir.absoluteFilePath(entry);
                QFileInfo entryInfo(fullPath);

                // 递归处理子项
                if (entryInfo.isDir() || entryInfo.isFile()) {
                    QString childText = traverseFolder(fullPath);
                    QJsonObject childNode =
                        QJsonDocument::fromJson(childText.toUtf8()).object();
                    sonArray.append(childNode);
                }
            }

            node["son"] = sonArray;
        } else {
            node["son"] = QJsonValue::Null;
        }
    } else {
        node["type"] = "file";
        node["son"] = QJsonValue::Null;
    }

    return QString(QJsonDocument(node).toJson());
}

