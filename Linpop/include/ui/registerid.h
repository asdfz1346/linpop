
#ifndef LINPOP_REGISTERID_H
#define LINPOP_REGISTERID_H

#include <QWidget>

namespace Ui {
class RegisterId;
}

class RegisterId : public QWidget {
    Q_OBJECT

public:
    explicit RegisterId(QWidget *ptParent = nullptr);
    ~RegisterId();

private slots:
    void on_logButton_clicked();
    void on_cancelButton_clicked();

private:
    void closeEvent(QCloseEvent* ptEvent);

    Ui::RegisterId* m_ptUi;
};

#endif // LINPOP_REGISTERID_H
