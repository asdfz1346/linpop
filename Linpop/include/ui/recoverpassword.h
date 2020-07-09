
#ifndef RECOVERPASSWORD_H
#define RECOVERPASSWORD_H

#include <QWidget>

namespace Ui {
class RecoverPassword;
}

class RecoverPassword : public QWidget {
    Q_OBJECT

public:
    explicit RecoverPassword(QWidget* ptParent = nullptr);
    ~RecoverPassword();

    void showPageByClient(const int iIndex);

private slots:
    void on_sureButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::RecoverPassword* m_ptUi;
};

#endif // RECOVERPASSWORD_H
