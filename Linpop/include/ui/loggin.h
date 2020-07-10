
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
    // 设置服务器ip文本框输入格式
    void setServerIpInputFormat(void);
    // 通过配置文件设置控件信息
    void setControlsByConfig(void);
    // 显示错误或警告信息
    void showTipWindow(const QString& rsTitle, const QString& rsTip,
                       const QString& rsButtonText = QStringLiteral("确定"));

/** 以下函数均为客户端操作 */
    // 设置SocketClient
    void setSocketClient(void);

public:
    // 根据注册界面信息拉取注册用户信息
    void sendToGetRegisterInfo(const QString& rsId,  const QString& rsPassword,
                               const QString& rsTip, const QString& rsName);
    // 根据账户和密码提示判断是否正确
    void sendToGetPosswordTip(const QString& rsId, const QString& rsTip);
    // 更新服务器中的密码
    void sendToGetModifyPossword(const QString& rsId, const QString& rsPassword);

private:
    // 根据账号密码拉取登录用户信息
    void sendToGetLoginUserInfo(void);
    // 根据账号拉取分组信息
    void sendToGetGroupMap(void);

    void parseRegisterInfo(const QJsonValue& rtData);
    void parsePosswordTip(const QJsonValue& rtData);
    void parsetModifyPossword(const QJsonValue& rtData);
    void parseLoginUserInfo(const QJsonValue& rtData);
    void parseGroupMap(const QJsonValue& rtData);

private slots:
    // 服务器消息处理
    void onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);
    // 服务器信息处理
    void onSigStatus(int reType/* const Sst& reType */);

private:
    Ui::Loggin*       m_ptUi;
    Friend*           m_ptFriend;
    RegisterId*       m_ptRegisterId;
    RecoverPassword*  m_ptRecoverPassword;
    QRegExpValidator* m_ptRxv;
    SocketClient*     m_ptSocketClient;
};

#endif // LINPOP_LOGGIN_H
