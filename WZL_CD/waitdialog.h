#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>

namespace Ui {
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitDialog(QWidget *parent = nullptr);
    ~WaitDialog();
    void config();
private:
    Ui::WaitDialog *ui;
private slots:
    void slot_down_wait_bar(int per);

};

#endif // WAITDIALOG_H
