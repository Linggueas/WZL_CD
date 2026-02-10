#ifndef CREATEDIRDIALOG_H
#define CREATEDIRDIALOG_H

#include <QDialog>

namespace Ui {
class CreateDirDialog;
}

class CreateDirDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateDirDialog(QWidget *parent = nullptr);
    ~CreateDirDialog();
signals:
    void sign_dir_name(const QString &dirname);
private slots:
    void on_pushButton_clicked();

private:
    Ui::CreateDirDialog *ui;
};

#endif // CREATEDIRDIALOG_H
