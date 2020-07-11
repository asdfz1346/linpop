
#ifndef LINPOP_SOCKETCLIENT_H
#define LINPOP_SOCKETCLIENT_H

typedef enum SOCKET_MESSAGE_TYPE {
    SMT_UNKNOW,

    SMT_REGISTER                = 0x10,     // 用户注册
    SMT_MATCHTIPS,                          // 验证密码提示
    SMT_MODIFYPASSWORD,                     // 修改密码
    SMT_LOGIN,                              // 用户登录
    SMT_LOGOUT,                             // 用户注销

    SMT_GETGROUP                = 0x20,     // 拉取分组信息
    SMT_ADDGROUP,
    SMT_RENAMEGROUP,
    SMT_DELGROUP,

    SMT_SEARCHFRIEND            = 0x30,     // 搜索好友
    SMT_ADDFRIENDSENDREQUEST,
    SMT_ADDFRIENDRECVREADY,

    SMT_ADDFRIEND               = 0x40,     // 添加好友
    SMT_MOVEFRIEND,
    SMT_DELFRIEND,
    SMT_GETFRIENDLIST,
} Smt;

typedef enum SOCKET_STATUS_TYPE {
    SST_UNKNOW,

    SST_CONNECTED               = 0x05,     // 用于发送连接信号
    SST_DISCONNECTED,                       // 用于发送取消连接信号

    SST_REGISTER_SUCCESS        = 0x10,     // 用户注册成功
    SST_REGISTER_FAILED,
    SST_MATCHTIPS_SUCCESS,                  // 匹配提示信息
    SST_MATCHTIPS_FAILED,
    SST_MODIFYPASSWORD_SUCCESS,
    SST_MODIFYPASSWORD_FAILED,
    SST_LOGIN_SUCCESS,                      // 用户登陆成功
    SST_PASSWORD_ERROR,                     // 密码错误
    SST_LOGIN_REPEAT,                       // 重复登录

    SST_GETGROUP_SUCCESS        = 0x25,     // 获取分组
    SST_GETGROUP_FAILED,
    SST_ADDGROUP_SUCCESS,                   // 添加分组
    SST_ADDGROUP_FAILED,
    SST_RENAMEGROUP_SUCCESS,                // 重命名分组
    SST_RENAMEGROUP_FAILED,
    SST_DELGROUP_SUCCESS,                   // 删除分组
    SST_DELGROUP_FAILED,

    SST_SEARCHFRIEND_SUCCESS    = 0x40,     // 搜索好友
    SST_SEARCHFRIEND_FAILED,
    SST_ADDFRIENDSENDREQUEST_SUCCESS,       // 添加好友请求
    SST_ADDFRIENDSENDREQUEST_FAILED,

    SST_ADDFRIEND_SUCCESS       = 0x45,     // 添加好友
    SST_ADDFRIEND_FAILED,
    SST_MOVEFRIEND_SUCCESS,                 // 移动好友
    SST_MOVEFRIEND_FAILED,
    SST_DELFRIEND_SUCCESS,                  // 删除还有
    SST_DELFRIEND_FAILED,
    SST_GETFRIENDLIST_SUCCESS,              // 获取好友列表
    SST_GETFRIENDLIST_FAILED,
} Sst;

#endif