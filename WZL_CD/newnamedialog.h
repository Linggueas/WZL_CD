#ifndef NEWNAMEDIALOG_H
#define NEWNAMEDIALOG_H

#include <QDialog>

namespace Ui {
class NewNameDialog;
}

class NewNameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewNameDialog(QWidget *parent = nullptr);
    ~NewNameDialog();

private slots:
    void on_pushButton_clicked();
signals:
    void sign_new_name(QString name);
private:
    Ui::NewNameDialog *ui;
};

#endif // NEWNAMEDIALOG_H
