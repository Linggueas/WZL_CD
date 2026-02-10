#include "log.h"
#include "QDateTime"
Logger* Logger::_log = nullptr;
Logger::Logger() {}
Logger::~Logger()
{
    //delete _log;
}
Logger* Logger::getinstance()
{
    if(_log == nullptr)
    {
        _log = new Logger();
    }
    return _log;
}
void Logger::write(const QString &_log_)
{
    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz");
    QString __log;
    __log.append(current_date);
    __log.append(" : ");
    __log.append(_log_);
    emit sign_write_log(__log);
}
