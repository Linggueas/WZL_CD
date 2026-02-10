#include "maindialog.h"
#include "ui_maindialog.h"
#include "createdirdialog.h"
#include "tcpmanger.h"
#include "QJsonDocument"
#include "QJsonObject"
#include "QJsonArray"
#include "global.h"
#include "m_dir.h"
#include "m_file.h"
#include "if_or_not_remove.h"
#include "QMessageBox"
#include "QFileDialog"
#include "QFile"
MainDialog::MainDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MainDialog)
{
    wait_dlg = new WaitDialog(this);
    m_cur_item = nullptr;
    m_cur_dir = "";
    ui->setupUi(this);
    add_menu = new QMenu(this);
    add_menu->addAction("创建文件夹");
    add_menu->addAction("添加文件");
    ui->CD_Widget->setColumnCount(1);
    ui->CD_Widget->setHeaderHidden(true);
    connect(ui->CD_Widget,&QTreeWidget::itemClicked,
            this,&MainDialog::onItemClicked);
    QJsonObject json_obj;
    json_obj["email"] = my_email;
    QByteArray data = QJsonDocument(json_obj).toJson();
    TcpManger::getInstance()->write(QString(data),ReqId::ID_REFRESH);
}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::on_create_dir_pushButton_clicked()
{
    if(m_cur_dir == "")
    {
        QMessageBox::critical(this,"错误","请选择路径");
        return;
    }
    QPoint pos =
        ui->create_dir_pushButton->
                 mapToGlobal(QPoint(0,ui->create_dir_pushButton->height()));
    connect(add_menu->actions().at(0),&QAction::triggered,this,[this](){
        CreateDirDialog *cd_dlg = new CreateDirDialog(this);
        cd_dlg->show();
        connect(cd_dlg,&CreateDirDialog::sign_dir_name,this,[this](const QString &dirname){
            QJsonObject json_obj;
            json_obj["email"] = my_email;
            //json_obj["dirname"] = dirname;
            QStringList stringlist;
            stringlist.append(m_cur_dir);
            stringlist.append(dirname);
            QString string = stringlist.join("/");
            json_obj["dirname"] = string;
            QTreeWidgetItem *item = new QTreeWidgetItem();
            m_dir*dir = new m_dir(this,dirname);
            m_cur_item->addChild(item);
            ui->CD_Widget->setItemWidget(item,0,dir);
            QByteArray data = QJsonDocument(json_obj).toJson();
            TcpManger::getInstance()->write(QString(data),ReqId::ID_CREATE_DIR);
        });

    });
    //创建文件夹
    connect(add_menu->actions().at(1),&QAction::triggered,this,[this](){
        QWidget* widget = ui->CD_Widget->itemWidget(m_cur_item,0);
        QString class_name = widget->metaObject()->className();
        if(class_name == "m_file")
        {
            QMessageBox::critical(this,"错误！","请选择文件夹");
            return;
        }
        QString file_dir = QFileDialog::getOpenFileName(
            this,  // 确保this指针有效
            tr("打开文件"),
            "D:/",
            tr("所有文件 (*)")
            );
        QFile file(file_dir);
        file.open(QIODevice::ReadOnly);
        QByteArray file_data = file.readAll();
        QStringList file_list = file_dir.split("/");
        QString file_name = file_list[file_list.size()-1];
        qDebug()<<file_name;
        QByteArray base64Data = file_data.toBase64();
        QByteArray decoded = QByteArray::fromBase64(base64Data);
        if (decoded != file_data) {
            qDebug() << "错误: Base64编码/解码不匹配!";
            qDebug() << "原始大小:" << file_data.size() << "解码大小:" << decoded.size();
            return;
        }
        QJsonObject json_obj;
        json_obj["email"] = my_email;
        json_obj["file_data"] = QString::fromUtf8(base64Data);
        json_obj["dir"] = m_cur_dir;
        json_obj["file_name"] = file_name;
        TcpManger::getInstance()->
            write(QString(QJsonDocument(json_obj).toJson()),ReqId::ID_CREATE_FILE);
        wait_dlg->show();
        connect(TcpManger::getInstance(),&TcpManger::sign_create_file,[this](QString data){
            wait_dlg->hide();
        });
    });
    add_menu->exec(pos);
    disconnect(add_menu->actions().at(0), &QAction::triggered, this, nullptr);
    disconnect(add_menu->actions().at(1), &QAction::triggered, this, nullptr);

}

//删除
void MainDialog::on_remove_dir_pushButton_clicked()
{
    if(m_cur_dir == "")
    {
        QMessageBox::critical(this,"错误","请选择路径");
        return;
    }
    if(m_cur_dir == my_email)
    {
        QMessageBox::critical(this,"错误","无法删除主文件夹");
        return;
    }
    if_or_not_remove*ionr = new if_or_not_remove(this);
    ionr->show();
    connect(ionr,&if_or_not_remove::sign_ok_remove,this,[this,ionr](){
        QJsonObject json_obj;
        json_obj["email"] = my_email;
        json_obj["dir"] = m_cur_dir;
        QWidget* widget = ui->CD_Widget->itemWidget(m_cur_item,0);
        QString type = widget->metaObject()->className();
        json_obj["type"] = type;
        QByteArray json_data = QJsonDocument(json_obj).toJson();
        TcpManger::getInstance()->write(QString(json_data),ReqId::ID_REMOVE);
        ionr->close();
    });
}

//点击cd后,获取现在的路径
void MainDialog::onItemClicked(QTreeWidgetItem *item, int column)
{
    m_cur_item = item;
    QTreeWidgetItem* current = item;
    QWidget* widget = ui->CD_Widget->itemWidget(item,0);
    QString class_name = widget->metaObject()->className();
    QStringList pathParts;
    if(class_name == "m_dir")
    {
        m_dir *now_dir = qobject_cast<m_dir*>(widget);
        QString now_dir_text = now_dir->GetText();
        pathParts.prepend(now_dir_text);
        while(current->parent())
        {
            current = current->parent();
            QWidget* widget = ui->CD_Widget->itemWidget(current,0);
            QString class_name = widget->metaObject()->className();
            if(class_name == "m_dir"){
                m_dir *now_dir = qobject_cast<m_dir*>(widget);
                QString now_dir_text = now_dir->GetText();
                pathParts.prepend(now_dir_text);
            }else if(class_name == "m_file"){
                m_file *now_file = qobject_cast<m_file*>(widget);
                QString now_file_text = now_file->GetText();
                pathParts.prepend(now_file_text);
            }
        }
        m_cur_dir = pathParts.join("/");
    }
    else if(class_name == "m_file"){
        m_file *now_file = qobject_cast<m_file*>(widget);
        QString now_file_text = now_file->GetText();
        pathParts.prepend(now_file_text);
        while(current->parent())
        {
            current = current->parent();
            QWidget* widget = ui->CD_Widget->itemWidget(current,0);
            QString class_name = widget->metaObject()->className();
            if(class_name == "m_dir"){
                m_dir *now_dir = qobject_cast<m_dir*>(widget);
                QString now_dir_text = now_dir->GetText();
                pathParts.prepend(now_dir_text);
            }else if(class_name == "m_file"){
                m_file *now_file = qobject_cast<m_file*>(widget);
                QString now_file_text = now_file->GetText();
                pathParts.prepend(now_file_text);
            }
        }
        m_cur_dir = pathParts.join("/");
    }
    qDebug()<<m_cur_dir;
}


//获取路径
QString MainDialog::getItemFullPath(QTreeWidgetItem* item)
{
    if (!item) return "";

    QStringList pathParts;
    QTreeWidgetItem* current = item;
    //QString name =
    // 添加当前项
    pathParts.prepend(current->text(0));

    // 向上遍历父项
    while (current->parent()) {
        current = current->parent();
        pathParts.prepend(current->text(0));
    }

    return pathParts.join("/");
}
//刷新
void MainDialog::on_refresh_pushButton_clicked()
{
    m_cur_item = nullptr;
    m_cur_dir = "";
    QJsonObject json_obj;
    json_obj["email"] = my_email;
    QByteArray data = QJsonDocument(json_obj).toJson();
    TcpManger::getInstance()->write(QString(data),ReqId::ID_REFRESH);
    connect(TcpManger::getInstance(),&TcpManger::sign_refresh,this,
            &MainDialog::slot_refresh);
}

//接收刷新请求的结构
void MainDialog::slot_refresh(QString data)
{
    ui->CD_Widget->clear();
    qDebug()<<data;
    QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
    QJsonArray json_arr = json_doc.array();
    add_refresh_item(json_arr,nullptr);
}

void MainDialog::add_refresh_item(QJsonArray json_arr,QTreeWidgetItem* const &item0)
{
    QTreeWidgetItem*_item = item0;
    for(int i = 0;i<json_arr.size();i++){
        QString name = json_arr.at(i).toObject()["name"].toString();
        QString type = json_arr.at(i).toObject()["type"].toString();
        if(item0 == nullptr){
            QTreeWidgetItem* item = new QTreeWidgetItem(ui->CD_Widget);
            m_dir *dir = new m_dir(this,name);
            item->setSizeHint(0,dir->size());
            ui->CD_Widget->setItemWidget(item,0,dir);
            _item = item;
        }else{
            if(type == "dir"){
                QTreeWidgetItem* item = new QTreeWidgetItem();
                m_dir *dir = new m_dir(this,name);
                item->setSizeHint(0,dir->size());
                item0->addChild(item);
                ui->CD_Widget->setItemWidget(item,0,dir);
                _item = item;
            }else if(type == "file"){
                QTreeWidgetItem* item = new QTreeWidgetItem();
                m_file *file = new m_file(this,name);
                item->setSizeHint(0,file->size());
                item0->addChild(item);
                ui->CD_Widget->setItemWidget(item,0,file);
                _item = item;
            }
        }
        auto json_arr0 = json_arr.at(i).toObject()["son"].toArray();
        if(json_arr0.isEmpty()){
            continue;
        }
        add_refresh_item(json_arr0,_item);
    }
}



//下载文件
void MainDialog::on_download_pushButton_clicked()
{
    QWidget* widget = ui->CD_Widget->itemWidget(m_cur_item,0);
    QString class_name = widget->metaObject()->className();
    if(class_name == "m_dir")
    {
        QMessageBox::critical(this,"错误！","请选择文件");
        return;
    }
    QJsonObject json_obj;
    json_obj["email"] = my_email;
    json_obj["dir"] = m_cur_dir;
    TcpManger::getInstance()->write(QString(QJsonDocument(json_obj).toJson()),ReqId::ID_DOWN_FILE);
    wait_dlg->show();
    connect(TcpManger::getInstance(),&TcpManger::sign_down_file,[this](QString data){
        QJsonDocument json_doc = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject json_obj0 = json_doc.object();
        auto file_name = json_obj0["file_name"].toString();
        auto file_data = json_obj0["file_data"].toString();
        QByteArray fileData = QByteArray::fromBase64(file_data.toUtf8());
        QString file_dir = QFileDialog::
            getSaveFileName(this,"保存文件",file_name,"所有文件 (*)");
        QFile file(file_dir);
        file.open(QIODevice::WriteOnly);
        file.write(fileData);
        wait_dlg->hide();
    });
    disconnect(TcpManger::getInstance(),&TcpManger::sign_down_file,this,nullptr);

}

//从命名
void MainDialog::on_reset_pushButton_clicked()
{
    NewNameDialog*nn_dlg = new NewNameDialog(this);
    nn_dlg->show();
    connect(nn_dlg,&NewNameDialog::sign_new_name,[this,nn_dlg](QString new_name){
        QJsonObject json_obj;
        json_obj["old_dir"] = m_cur_dir;
        json_obj["new_name"] = new_name;
        TcpManger::getInstance()->write(QString(QJsonDocument(json_obj).toJson()),ReqId::ID_NEW_NAME);
        nn_dlg->hide();
    });

}

