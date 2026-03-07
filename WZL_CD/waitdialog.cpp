#include "waitdialog.h"
#include "ui_waitdialog.h"
#include "tcpmanger.h"
WaitDialog::WaitDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WaitDialog)
{
    ui->setupUi(this);
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(0);
    ui->progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 1px solid grey;"          // 边框
        "    border-radius: 5px;"              // 外框圆角
        "    background-color: #e0e0e0;"        // 背景色
        "    text-align: center;"               // 文本对齐
        "    height: 20px;"                     // 高度
        "}"
        "QProgressBar::chunk {"
        "    border-radius: 5px;"               // 进度块圆角
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0,"
        "                                stop:0 #78d, stop:1 #5bf);" // 渐变色
        "}");
    connect(&TcpManger::getInstance(),&TcpManger::sign_down_wait_bar,this,&WaitDialog::slot_down_wait_bar);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::config()
{
    ui->progressBar->setValue(0);
}

void WaitDialog::slot_down_wait_bar(int per)
{
    ui->progressBar->setValue(per);
}
