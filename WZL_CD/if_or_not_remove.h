#ifndef IF_OR_NOT_REMOVE_H
#define IF_OR_NOT_REMOVE_H

#include <QDialog>
#include <QAbstractButton>
namespace Ui {
class if_or_not_remove;
}

class if_or_not_remove : public QDialog
{
    Q_OBJECT

public:
    explicit if_or_not_remove(QWidget *parent = nullptr);
    ~if_or_not_remove();

private slots:


    void on_buttonBox_clicked(QAbstractButton *button);

private:
    Ui::if_or_not_remove *ui;
signals:
    void sign_ok_remove();
};

#endif // IF_OR_NOT_REMOVE_H
