
#ifndef LINPOP_LOGGIN_H
#define LINPOP_LOGGIN_H

#include <moveableframelesswindow.h>
#include <singleton.h>
#include <friend.h>
#include <recoverpassword.h>
#include <registerid.h>
#include <socketclient.h>

#include <QWidget>
#include <QString>
#include <QRegExpValidator>

namespace Ui {
class Loggin;
}

class Loggin : public MoveableFramelessWindow, public Singleton<Loggin> {
    Q_OBJECT

public:
    ~Loggin();
    void setEditText(const QString& rsId, const QString& rsPassword);

protected:
    friend class Singleton<Loggin>;
    explicit Loggin(QWidget *ptParent = nullptr);

    QWidget* getDragnWidget();

private slots:
    void on_setButton_clicked();
    void on_minButton_clicked();
    void on_closeButton_clicked();
    void on_logButton_clicked();
    void on_cancelButton_clicked();
    void on_losePasswordButton_clicked();
    void on_newButton_clicked();
    void on_sureSetButton_clicked();
    void on_cancelSetButton_clicked();

private:
    void setServerIpInputFormat(void);
    void setControlsByConfig(void);

    void sendIdAndPassword(void);
    void recvUserInfo(void);

    Ui::Loggin*       m_ptUi;
    Friend*           m_ptFriend;
    RegisterId*       m_ptRegisterId;
    RecoverPassword*  m_ptRecoverPassword;
    QRegExpValidator* m_ptRxv;
    SocketClient*     m_ptSocketClient;
};

#endif // LINPOP_LOGGIN_H
