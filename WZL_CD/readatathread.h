#ifndef READATATHREAD_H
#define READATATHREAD_H

#include <QObject>
#include <QThread>
class readatathread : public QThread
{
    Q_OBJECT
public:
    explicit readatathread(QObject *parent = nullptr);
private:
    void run()override;
signals:
};

#endif // READATATHREAD_H
