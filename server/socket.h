
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

    SMT_UPDATENAME              = 0x50,     // 修改昵称
    SMT_UPDATEHEAD,                         // 修改头像

    SMT_UPDATEFRIENDSTATUS      = 0x55,     // 更新好友状态

    SMT_SENDMESSAGE             = 0x60,     // 发送消息
    SMT_RECVMESSAGE,

    SMT_GETHISTORY              = 0x65,     // 获取历史消息
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
    SST_DELFRIEND_SUCCESS,                  // 删除好友
    SST_DELFRIEND_FAILED,
    SST_GETFRIENDLIST_SUCCESS,              // 获取好友列表
    SST_GETFRIENDLIST_FAILED,

    SST_UPDATENAME_SUCCESS      = 0x55,     // 修改昵称
    SST_UPDATENAME_FAILED,
    SST_UPDATEHEAD_SUCCESS,                 // 修改头像
    SST_UPDATEHEAD_FAILED,

    SST_UPDATEFRIENDSTATUS_SUCCESS  = 0x60,
    SST_UPDATEFRIENDSTATUS_FAILED,

    SST_SENDMESSAGE_SUCCESS     = 0x65,     // 发送消息
    SST_SENDMESSAGE_FAILED,
    SST_RECVMESSAGE_SUCCESS,
    SST_RECVMESSAGE_FAILED,

    SST_GETHISTORY_SUCCESS      = 0x70,     // 获取历史消息
    SST_GETHISTORY_FAILED,
} Sst;

typedef enum SOCKET_CHAT_MESSAGE_TYPE {
    SCMT_UNKNOW,
    SCMT_HEAD                    = 0x10,
    SCMT_STRING,
    SCMT_PICTURE,
    SCMT_OTHERFILE,
} Scmt;

typedef struct fileinfo{
    char FileName[100];
    char TargetId[52];
    int FileLen;
}FILEINFO;
typedef enum SOCKET_FILE_STATUS_TYPE {
    SFST_UNKNOW,

    SFST_CONNECTED              = 0x90,     // 用于发送连接信号
    SFST_DISCONNECTED,                      // 用于发送取消连接信号

    SFST_SENDFILE_SUCCESS       = 0x95,     // 发送成功
    SFST_SENDFILE_FAILED,
    SFST_RECVFILE_SUCCESS,                  // 接收成功
    SFST_RECVFILE_FAILED,
} Sfst;
typedef enum SOCKET_FILE_TYPE {
    SFT_UNKNOW,

    SFT_SENDFILE                = 0x90,     // 发送文件
    SFT_RECVFILE,                           // 接收文件
} Slt;

#endif
