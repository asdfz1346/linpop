
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

class RegisterId;

class Loggin : public MoveableFramelessWindow, public Singleton<Loggin> {
    Q_OBJECT

public:
    ~Loggin();
    // 向服务器发送注册信息
    void sendRegisterInfo(const QString& rsId,  const QString& rsPassword,
                          const QString& rsTip, const QString& rsName);

protected:
    friend class Singleton<Loggin>;
    explicit Loggin(QWidget *ptParent = nullptr);

    QWidget* getDragnWidget();

private slots:
    // 设置按键
    void on_setButton_clicked();
    // 最小化按键
    void on_minButton_clicked();
    // 右上角关闭按键
    void on_closeButton_clicked();
    // 登录按键
    void on_logButton_clicked();
    // 取消按键
    void on_cancelButton_clicked();
    // 找回密码按键
    void on_losePasswordButton_clicked();
    // 注册账号按键
    void on_newButton_clicked();
    // 确定设置服务器ip按键
    void on_sureSetButton_clicked();
    // 取消设置服务器ip按键
    void on_cancelSetButton_clicked();
    // 服务器登录信息处理
    void onSigStatus(int reType/* const Sst& reType */);

private:
    // 设置服务器ip文本框输入格式
    void setServerIpInputFormat(void);
    // 通过配置文件设置控件信息
    void setControlsByConfig(void);
    // 设置SocketClient
    void setSocketClient(void);

    // 向服务器发送账号和密码，用于登陆操作
    void sendIdAndPassword(void);
    // 向服务器发送账号，用于获取分组
    void sendId(void);
    void recvUserInfo(void);

    void showTipWindow(const QString& rsTitle, const QString& rsTip,
                       const QString& rsButtonText = QStringLiteral("确定"));

    Ui::Loggin*       m_ptUi;
    Friend*           m_ptFriend;
    RegisterId*       m_ptRegisterId;
    RecoverPassword*  m_ptRecoverPassword;
    QRegExpValidator* m_ptRxv;
    SocketClient*     m_ptSocketClient;
};

#endif // LINPOP_LOGGIN_H
