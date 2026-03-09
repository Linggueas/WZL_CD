#include "global.h"

QMap<QString,QTcpSocket*>socket;
QMap<QTcpSocket*,QByteArray> _buffer;
QMap<QTcpSocket*,readDataThread*>_rd_thread;
QMutex mutex;
QMutex log_mutex;
