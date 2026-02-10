#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include<global.h>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private slots:
    void on_return_button_clicked();

    void on_var_pushButton_clicked();

    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
    void on_pushButton_2_clicked();

signals:
    void sign_switch_login();
private:
    Ui::RegisterDialog *ui;
    QString var;

};

#endif // REGISTERDIALOG_H
