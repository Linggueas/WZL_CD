#ifndef DBMGR_H
#define DBMGR_H

#include <QObject>
#include<QSqlDatabase>
#include<QSqlQuery>
class DBMgr : public QObject
{
    Q_OBJECT
public:
    explicit DBMgr(QObject *parent = nullptr);
    static DBMgr*GetInstance();
    void insert(const QString &table,const QStringList &Column,const QStringList &newdata);
    bool update(const QString &table, const QString &cond1,const QString &cond);
private:
    QSqlDatabase sql_db;
    static DBMgr *dbmgr;
signals:
};

#endif // DBMGR_H
