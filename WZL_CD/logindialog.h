#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
signals:
    void sign_switch_register();
    void sign_switch_main();
private slots:
    void on_regis_Button_clicked();
    void on_login_Button_clicked();
    void slot_login_mod_finish(ReqId id, QString res, ErrorCodes err);
    void slot_switch_main();

};

#endif // LOGINDIALOG_H
