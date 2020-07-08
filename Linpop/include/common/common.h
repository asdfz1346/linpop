
#ifndef LINPOP_COMMON_H
#define LINPOP_COMMON_H

#include <config.h>

#include <QString>
#include <QStringList>

struct UserInfo {
    QString sId;                        // 账号
    QString sPassword;			// 密码
    QString sIp;                        // ip
    QString sName;                      // 昵称
    QString sHead;                      // 头像路径
};

struct FriendInfo {
    bool    bOnline;			// 是否在线
    int     iGroup;                     // 所属分组的SQL index
    QString sId;                        // 账号
    QString sIp;                        // ip
    QString sName;                      // 昵称
    QString sHead;                      // 头像路径
};

extern UserInfo    g_tMyselfInfo;       // 个人信息
extern QString     g_tServerIpAddr;     // 服务器地址
extern QStringList g_lsGroupTextList;   // 分组名，只限于Group和GroupItem使用

#endif // LINPOP_COMMON_H
