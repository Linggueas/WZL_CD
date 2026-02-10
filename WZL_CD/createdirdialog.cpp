#include "createdirdialog.h"
#include "ui_createdirdialog.h"

CreateDirDialog::CreateDirDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CreateDirDialog)
{
    ui->setupUi(this);
}

CreateDirDialog::~CreateDirDialog()
{
    delete ui;
}

void CreateDirDialog::on_pushButton_clicked()
{
    emit sign_dir_name(ui->lineEdit->text());
    close();
}

