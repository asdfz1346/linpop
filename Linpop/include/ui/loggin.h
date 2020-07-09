
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
    // 根据注册界面信息拉取注册用户信息
    void sendToGetRegisterInfo(const QString& rsId,  const QString& rsPassword,
                               const QString& rsTip, const QString& rsName);
    // 根据账户和密码提示判断是否正确
    void sendToGetPosswordTip(const QString& rsId, const QString& rsTip);
    // 更新服务器中的密码
    void sendToGetModifyPossword(const QString& rsId, const QString& rsPassword);

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
    // 服务器消息处理
    void onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);
    // 服务器信息处理
    void onSigStatus(int reType/* const Sst& reType */);

private:
    // 设置服务器ip文本框输入格式
    void setServerIpInputFormat(void);
    // 通过配置文件设置控件信息
    void setControlsByConfig(void);
    // 显示错误或警告信息
    void showTipWindow(const QString& rsTitle, const QString& rsTip,
                       const QString& rsButtonText = QStringLiteral("确定"));

    // 设置SocketClient
    void setSocketClient(void);
    // 根据账号密码拉取登录用户信息
    void sendToGetUserInfo(void);
    // 根据账号拉取分组信息
    void sendToGetGroupList(void);

    // 解析匹配信息
    void parseMatchTipsStatus(const QJsonValue& rtData);
    // 解析修改密码信息
    void parseModifyPasswordStatus(const QJsonValue& rtData);

    Ui::Loggin*       m_ptUi;
    Friend*           m_ptFriend;
    RegisterId*       m_ptRegisterId;
    RecoverPassword*  m_ptRecoverPassword;
    QRegExpValidator* m_ptRxv;
    SocketClient*     m_ptSocketClient;
};

#endif // LINPOP_LOGGIN_H
