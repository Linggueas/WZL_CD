#ifndef DOWNTASK_H
#define DOWNTASK_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
class downTask : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit downTask(QObject *parent = nullptr);
private:
    QTcpSocket
    void run() override;

signals:
};

#endif // DOWNTASK_H
