#include "newnamedialog.h"
#include "ui_newnamedialog.h"

NewNameDialog::NewNameDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewNameDialog)
{
    ui->setupUi(this);
}

NewNameDialog::~NewNameDialog()
{
    delete ui;
}

void NewNameDialog::on_pushButton_clicked()
{
    QString new_name = ui->lineEdit->text();
    emit sign_new_name(new_name);
}

