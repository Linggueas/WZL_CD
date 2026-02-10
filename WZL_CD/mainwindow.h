#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "logindialog.h"
#include "registerdialog.h"
#include "maindialog.h"
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

private slots:
    void on_pushButton_clicked();
    void slot_switch_register();
    void slot_switch_login();
    void slot_switch_main();
private:
    Ui::MainWindow *ui;
    LoginDialog*log_dlg;
    RegisterDialog*regis_dlg;
    MainDialog*main_dlg;
};
#endif // MAINWINDOW_H
