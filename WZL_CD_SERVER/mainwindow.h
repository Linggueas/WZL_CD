#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<tcpmanger.h>
#include"dbmgr.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    TcpManger*tcp_manger;
private slots:
    void sign_wirte_log(const QString &__log);
};
#endif // MAINWINDOW_H
