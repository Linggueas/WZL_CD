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
            qDebug() << "receive body msg is " << messageBody;

            _buffer = _buffer.mid(_message_len);
            emit taskFinished(messageBody,_message_id);
        }
    },Qt::QueuedConnection);
	exec(); 
}
