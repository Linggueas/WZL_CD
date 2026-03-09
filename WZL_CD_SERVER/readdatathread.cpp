#include "readdatathread.h"

readDataThread::readDataThread(QObject *parent,QTcpSocket*tcpsock)
    : QThread{parent}
{
    this->tcpsock = tcpsock;
    tcpsock->moveToThread(this);
    _buffer = "";
    _b_recv_pending = false;
    _message_id = 0;
    _message_len = 0;
    per = 0;
    initfunction_map();
}
void readDataThread::run()
{
    qDebug()<<"====="<<QThread::currentThreadId();
    connect(tcpsock,&QTcpSocket::readyRead,this,[this](){
        _buffer.append(tcpsock->readAll());
        QByteArray messageBody;
        QDataStream stream(&_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_6_0);
        forever{
            if (!_b_recv_pending) {

                if (_buffer.size() < static_cast<int>(sizeof(quint16) * 2)) {
                    return; // 数据不够，等待更多数据
                }


                stream >> _message_id >> _message_len;

                //将buffer 中的前四个字节移除
                _buffer = _buffer.mid(sizeof(quint16) +sizeof(long long));

                // 输出读取的数据
                qDebug() << "Message ID:" << _message_id << ", Length:" << _message_len;

            }






            //buffer剩余长读是否满足消息体长度，不满足则退出继续等待接受
            if (_buffer.size() < _message_len) {
                _b_recv_pending = true;
                return;
            }
            _b_recv_pending = false;
            // 读取消息体
            messageBody = _buffer.mid(0, _message_len);
            per = 0;
            _buffer = _buffer.mid(_message_len);
            auto find_iter = function_map.find((ReqId)_message_id);
            if (find_iter == function_map.end()) {
                qDebug() <<_message_id;
                qDebug() << "not found id ";
                return;
            }

            find_iter.value()(tcpsock,messageBody);

        }
    },Qt::QueuedConnection);
	exec(); 
}


void readDataThread::initfunction_map()
{

    //登录
    function_map.insert(ReqId::ID_LOGIN_USER,[this](QTcpSocket *tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        mutex.lock();
        socket.insert(email,tcpsock);
        mutex.unlock();
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
        log_mutex.lock();
        Logger::getinstance()->
            write(QString("%1创建文件夹%2").arg(email).arg(dirname));
        log_mutex.unlock();
        QDir dir;
        bool error = dir.mkdir(dirname);

    });
    //刷新
    function_map.insert(ID_REFRESH,[this](QTcpSocket*tcpsock,QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj = json_doc.object();
        auto email = json_obj["email"].toString();
        QString json_data =  traverseFolder(email);
        log_mutex.lock();
        Logger::getinstance()->
            write(QString("%1请求刷新").arg(email));
        log_mutex.unlock();
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
        log_mutex.lock();
        Logger::getinstance()->write(QString("%1 删除文件/文件夹：%2").arg(email).arg(dir));
        log_mutex.unlock();
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
        log_mutex.lock();
        Logger::getinstance()->write(QString("%1 添加文件/文件夹：%2/%3").arg(email).arg(dir).arg(file_name));
        log_mutex.unlock();
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
        log_mutex.lock();
        Logger::getinstance()->
            write(QString("%1下载文件%2").arg(email).arg(dirlist[dirlist.size()-1]));
        log_mutex.unlock();
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
void readDataThread::write(const QString &data,ReqId reqid,QTcpSocket *tcpsock)
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
QString readDataThread::traverseFolder(const QString &folderPath) {
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
















