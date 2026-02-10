#include "m_dir.h"
#include "ui_m_dir.h"
#include"QPixmap"
m_dir::m_dir(QWidget *parent,const QString &dirname)
    : QMainWindow(parent)
    , ui(new Ui::m_dir)
{
    ui->setupUi(this);
    QPixmap pixmap(":/new/prefix1/dir.png");
    pixmap = pixmap.scaled(ui->label->size());
    ui->label->setPixmap(pixmap);
    ui->label_2->setText(dirname);
}

m_dir::~m_dir()
{
    delete ui;
}

QString m_dir::GetText()
{
    return ui->label_2->text();
}
