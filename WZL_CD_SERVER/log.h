#ifndef LOG_H
#define LOG_H

#include <QObject>
class Logger : public QObject
{
    Q_OBJECT
public:
    Logger();
    ~Logger();
    static Logger*getinstance();
    void write(const QString &_log_);
private:
    static Logger*_log;
signals:
    void sign_write_log(const QString &_log_);
};

#endif // LOG_H
