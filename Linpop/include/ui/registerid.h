
#ifndef LINPOP_REGISTERID_H
#define LINPOP_REGISTERID_H

#include <loggin.h>

#include <QWidget>
#include <QString>

namespace Ui {
class RegisterId;
}

class Loggin;

class RegisterId : public QWidget {
    Q_OBJECT

public:
    explicit RegisterId(QWidget *ptParent = nullptr);
    ~RegisterId();

    QString getIdEditText(void);

private slots:
    void on_logButton_clicked();
    void on_cancelButton_clicked();

private:
    // 关闭按键按下返回上一层
    void closeEvent(QCloseEvent* ptEvent);

    Ui::RegisterId* m_ptUi;
    Loggin*         m_ptLoggin;
};

#endif // LINPOP_REGISTERID_H
