#include "if_or_not_remove.h"
#include "ui_if_or_not_remove.h"
if_or_not_remove::if_or_not_remove(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::if_or_not_remove)
{
    ui->setupUi(this);
}

if_or_not_remove::~if_or_not_remove()
{
    delete ui;
}



void if_or_not_remove::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
        emit sign_ok_remove();

    }
    if(ui->buttonBox->buttonRole(button) == QDialogButtonBox::RejectRole)
    {
        close();
    }
}

