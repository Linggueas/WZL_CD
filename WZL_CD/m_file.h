#ifndef M_FILE_H
#define M_FILE_H

#include <QMainWindow>

namespace Ui {
class m_file;
}

class m_file : public QMainWindow
{
    Q_OBJECT

public:
    explicit m_file(QWidget *parent = nullptr,const QString &filename = "");
    ~m_file();
    QString GetText();
private:
    Ui::m_file *ui;

};

#endif // M_FILE_H
