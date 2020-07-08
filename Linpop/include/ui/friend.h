
#ifndef LINPOP_FRIEND_H
#define LINPOP_FRIEND_H

#include <singleton.h>
#include <common.h>
#include <group.h>
#include <groupitem.h>
#include <socketclient.h>

#include <QWidget>
#include <QStringList>
#include <QList>

/**
 * UI eg:
 * +------------------------+
 * | Friend                 |
 * | +--------------------+ |
 * | | Group              | |
 * | | +----------------+ | |
 * | | | GroupItem      | | |
 * | | | +------------+ | | |
 * | | | | FriendItem | | | |
 * | | | +------------+ | | |
 * | | | | FriendItem | | | |
 * | | | +------------+ | | |
 * | | +----------------+ | |
 * | | | GroupItem      | | |
 * | | | +------------+ | | |
 * | | | | FriendItem | | | |
 * | | | +------------+ | | |
 * | | +----------------+ | |
 * | +--------------------+ |
 * +------------------------+
 */

namespace Ui {
class Friend;
}

class Friend : public QWidget, public Singleton<Friend> {
    Q_OBJECT

public:
    ~Friend();

    void preInit(/*const UserInfo& rtMyselfInfo*/);
    void postInit(/*const UserInfo& rtMyselfInfo*/);

    // 与数据库的交互函数
    void pushUserInfo(/*const UserInfo& rtMyselfInfo*/);

    int  pullGroupItemList(/*const UserInfo& rtMyselfInfo*/);
    void pushGroupItemList(/*const UserInfo& rtMyselfInfo*/);

    void pullFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);
    void pushFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    GroupItem* getGroupitemIndex(const int iIndex);

protected:
    friend class Singleton<Friend>;
    explicit Friend(QWidget *ptParent = nullptr);

private slots:
    // 修改头像按键
    void on_headButton_clicked();
    // 服务器消息处理
    void onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);
    // 服务器信息处理
    void onSigStatus(int reType/* const Sst& reType */);

private:
    // 分组操作
    void addGroupItem(const QString& rsName = GROUPITEM_NAME_DEFAULT);
    void renameGroupItem(const int iIndex);
    void delGroupItem(const int iIndex);
    // 好友操作，添加好友直接添加到iIndex分组的末尾
    void addFriendItem(const int iIndex, const FriendInfo& rtFriendInfo);
    void moveFriendItem(const int iSrcIndex, const int iDestIndex, const int iIndex,
                        bool isFriendListAppend = true);
    void delFriendItem(const int iGroupIndex, const int iIndex);

    // 设置控件
    void initUserInofControls(/*const UserInfo& rtMyselfInfo*/);
    void initGroupItemControls(/*const UserInfo& rtMyselfInfo*/);
    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);
    // 设置SocketClient
    void setSocketClient(void);
    // 向服务器发送账号和分组index，服务器返回好友列表（暂时未改名）
    void sendIdAndGroupIndex(const int iGroupIndex);
    // 向服务器发送id查询好友是否存在
    // 向服务器发送添加好友请求
    void getFriendInfoBySendId(const int iGroupIndex);

    // 解析获取好友列表返回信息
    void parseOneGroupFriend(const QJsonValue& rtData);
    // 解析添加好友返回信息
    void parseOneFriend(const QJsonValue& rtData);
    // 解析并构造好友信息
    void parseFriendInfo(const QJsonValue& rtData, FriendInfo& rtFriendInfo);

    Ui::Friend*   m_ptUi;
    Group*        m_ptGroup;
    SocketClient* m_ptSocketClient;
};

#endif // LINPOP_FRIEND_H
