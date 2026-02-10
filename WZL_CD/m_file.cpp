#include "m_file.h"
#include "ui_m_file.h"

m_file::m_file(QWidget *parent,const QString &filename)
    : QMainWindow(parent)
    , ui(new Ui::m_file)
{
    ui->setupUi(this);
    QPixmap pixmap(":/new/prefix1/file.png");
    pixmap = pixmap.scaled(ui->label->size());
    ui->label->setPixmap(pixmap);
    ui->label_2->setText(filename);
}

m_file::~m_file()
{
    delete ui;
}
QString m_file::GetText()
{
    return ui->label_2->text();
}
