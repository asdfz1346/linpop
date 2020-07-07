
#ifndef LINPOP_SOCKETCLIENT_H
#define LINPOP_SOCKETCLIENT_H

#include <config.h>

#include <QString>
#include <QTcpSocket>
#include <QJsonValue>

typedef enum SOCKET_MESSAGE_TYPE {
    Unknow,
    Register            = 0x10,         // 用户注册
    Login,                              // 用户登录
    Logout,                             // 用户注销
    LoginRepeat,                        // 重复登录
    MatchTips,                          // 验证密码提示
    ModifyPassword,                     // 修改密码
} Smt;

/**
 * 准备改成单例模式
 */
class SocketClient : public QObject {
public:
    explicit SocketClient(QObject* ptParent = nullptr);
    ~SocketClient();

    void checkConnect(void);
    void closeConnect(void);

    // 连接服务器
    void connectToServer(const QString& rsServerIpAddr = SOCKET_SERVER_DEFAULT, const int iPort = SOCKET_PORT_DEFAULT);

signals:
    void signalMessage(const Smt& reType, const QJsonValue& rtData);
    void signalStatus(const Smt& reType);

public slots:
    // 发送消息函数
    void onSendMessage(const Smt& reType, const QJsonValue& rtData);
    // 上线通知
    void onSendOnlineMessage(void);
    // 下线通知
    void onSendOfflineMessage(void);

private:
    QTcpSocket* m_ptTcpSocket;

private slots:
    // 与服务器断开链接
    void onDisconnect(void);
    // 链接上服务器
    void onConnect(void);
    // tcp消息处理
    void onReadyRead(void);
#if 0
private:
    // 解析登陆返回信息
    void ParseLogin(const QJsonValue &dataVal);
    // 解析注册返回信息
    void ParseReister(const QJsonValue &dataVal);
#endif
};

#endif // LINPOP_SOCKETCLIENT_H
