#ifndef M_DIR_H
#define M_DIR_H

#include <QMainWindow>

namespace Ui {
class m_dir;
}

class m_dir : public QMainWindow
{
    Q_OBJECT

public:
    explicit m_dir(QWidget *parent = nullptr,const QString &dirname = "");
    ~m_dir();
    QString GetText();
private:
    Ui::m_dir *ui;
};

#endif // M_DIR_H
