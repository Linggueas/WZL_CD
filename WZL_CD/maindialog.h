#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include <QMenu>
#include <QTreeWidget>
#include "waitdialog.h"
#include "newnamedialog.h"
#include "tcpmanger.h"
#include "QThread"
#include "readatathread.h"
namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr);
    ~MainDialog();

private slots:
    void on_create_dir_pushButton_clicked();

    void on_remove_dir_pushButton_clicked();

    void onItemClicked(QTreeWidgetItem* item, int column);

    void on_refresh_pushButton_clicked();
    void slot_refresh(QString data);

    void on_download_pushButton_clicked();

    void on_reset_pushButton_clicked();

private:
    Ui::MainDialog *ui;
    QMenu*add_menu;
    QAction *dir_act;
    QAction *file_act;
    QString m_cur_dir;
    QTreeWidgetItem*m_cur_item;
    QString getItemFullPath(QTreeWidgetItem* item);
    void add_refresh_item(QJsonArray json_arr,QTreeWidgetItem* const &item);
    WaitDialog *wait_dlg;
    readatathread *thread;
signals:
    void sign_write(const QString &data,ReqId reqid);
};

#endif // MAINDIALOG_H
