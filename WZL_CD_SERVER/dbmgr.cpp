#include "dbmgr.h"
#include "log.h"
DBMgr* DBMgr::dbmgr = nullptr;
DBMgr::DBMgr(QObject *parent)
    : QObject{parent}
{
    sql_db = QSqlDatabase::addDatabase("QMYSQL");
    sql_db.setHostName("127.0.0.1");
    sql_db.setUserName("root");
    sql_db.setPassword("20040724qwe");
    sql_db.setPort(3306);
    sql_db.setDatabaseName("wzl_cd_sql");
    if(sql_db. open())
    {
        Logger::getinstance()->write("数据库连接成功");
    }else{
        Logger::getinstance()->write(" 数据库连接失败");
    }
}


//
DBMgr* DBMgr::GetInstance()
{
    if(dbmgr==nullptr)
    {
        dbmgr = new DBMgr();
    }
    return dbmgr;
}
//insert
void DBMgr::insert(const QString &table,
                   const QStringList &Column,
                   const QStringList &newdata)
{

}
//update
bool DBMgr::update(const QString &table,
                   const QString &cond1,
                   const QString &cond2)
{
    QString sql_data = "update %1 "
                       "set %2 "
                       "where %3";
    sql_data = sql_data.arg(table).arg(cond1).arg(cond2);
    QSqlQuery sql_que;
    return sql_que.exec(sql_data);
}
