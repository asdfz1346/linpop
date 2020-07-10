﻿
#ifndef LINPOP_SOCKETCLIENT_H
#define LINPOP_SOCKETCLIENT_H

#include <singleton.h>
#include <config.h>

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QJsonValue>

typedef enum SOCKET_MESSAGE_TYPE {
    SMT_UNKNOW,

    SMT_REGISTER            = 0x10,         // 用户注册
    SMT_LOGIN,                              // 用户登录
    SMT_LOGOUT,                             // 用户注销
//    SMT_GETUSERINFO,                        // 获取用户信息，移动至SMT_LOGIN事件中
    SMT_MATCHTIPS,                          // 验证密码提示
    SMT_MODIFYPASSWORD,                     // 修改密码

    SMT_GETGROUP            = 0x20,         // 拉取分组信息
    SMT_ADDGROUP,
    SMT_DELGROUP,
    SMT_RENAMEGROUP,

    SMT_GETFRIENDLIST,                      // 拉取好友信息
    SMT_SEARCHFRIEND,
    SMT_ADDFRIENDSENDREQUEST,
    SMT_ADDFRIENDRECVREQUEST,
    SMT_ADDFRIEND,
    SMT_DELFRIEND,
    SMT_MOVEFRIEND,
    SMT_MOVEALLFRIEND,

} Smt;

typedef enum SOCKET_STATUS_TYPE {
    SST_UNKNOW,

    SST_CONNECTED               = 0x10,     // 用于发送连接信号
    SST_DISCONNECTED,                       // 用于发送取消连接信号

    SST_LOGIN_SUCCESS           = 0x15,     // 用户登陆成功
    SST_PASSWORD_ERROR,                     // 密码错误
    SST_LOGIN_REPEAT,                       // 重复登录

    SST_REGISTER_SUCCESS        = 0x20,     // 用户注册成功
    SST_REGISTER_FAILED,

    SST_MATCHTIPS_SUCCESS       = 0x25,     // 匹配提示信息
    SST_MATCHTIPS_FAILED,

    SST_MODIFYPASSWORD_SUCCESS  = 0x30,     // 修改密码
    SST_MODIFYPASSWORD_FAILED,

    SST_GETGROUP_SUCCESS        = 0x35,     // 获取分组
    SST_GETGROUP_FAILED,
    SST_ADDGROUP_SUCCESS,                   // 添加分组
    SST_ADDGROUP_FAILED,
    SST_RENAMEGROUP_SUCCESS,                // 重命名分组
    SST_RENAMEGROUP_FAILED,
    SST_DELGROUP_SUCCESS,                   // 删除分组
    SST_DELGROUP_FAILED,

    SST_GETFRIEND_SUCCESS       = 0x45,     // 获取好友列表
    SST_GETFRIEND_FAILED,
    SST_SEARCHFRIEND_SUCCESS,               // 搜索好友
    SST_SEARCHFRIEND_FAILED,
    SST_ADDFRIENDSENDREQUEST_SUCCESS,       // 添加好友请求
    SST_ADDFRIENDSENDREQUEST_FAILED,
    SST_ADDFRIENDRECVREQUEST_SUCCESS,       // 接收添加好友请求
    SST_ADDFRIENDRECVREQUEST_FAILED,
    SST_ADDFRIEND_SUCCESS,                  // 添加好友
    SST_ADDFRIEND_FAILED,
    SST_MOVEFRIEND_SUCCESS,                 // 移动好友
    SST_MOVEFRIEND_FAILED,
    SST_DELFRIEND_SUCCESS,                  // 删除还有
    SST_DELFRIEND_FAILED,
} Sst;

class SocketClient : public QObject, public Singleton<SocketClient> {
    Q_OBJECT

public:
    ~SocketClient();
    // 连接服务器操作
    void startConnect(const QString& rsServerIpAddr = SOCKET_SERVER_DEFAULT,
                      const int iPort = SOCKET_PORT_DEFAULT);
    bool checkConnect(void);
    void closeConnect(void);

signals:
    // 让下层自己对消息进行判断
    void sigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);
    // 向下层发送成功或失败的状态信号
    void sigStatus(int reType/* const Sst reType */);

public slots:
    // 发送消息函数
    void onSendMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);

protected:
    friend class Singleton<SocketClient>;
    explicit SocketClient(QObject* ptParent = nullptr);

private slots:
    // 接收消息函数
    void onReadyRead();
    // 连接服务器的槽函数
    void onConnected();
    // 断开服务器的槽函数
    void onDisConnected();

private:
    QTcpSocket* m_ptTcpSocket;
    bool        m_bIsConnected;
};

#endif // LINPOP_SOCKETCLIENT_H
