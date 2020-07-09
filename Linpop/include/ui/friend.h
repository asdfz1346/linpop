
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
    // 预初始化
    void preInit(/*const UserInfo& rtMyselfInfo*/);
    void postInit(/*const UserInfo& rtMyselfInfo*/);

    GroupItem* getGroupitemIndex(const int iIndex);

    // 分组操作，添加分组直接添加到分组列表的末尾
    void addGroupItem(const QString& rsName = GROUPITEM_NAME_DEFAULT);
    void renameGroupItem(const int iIndex, const QString& rsName);
    void delGroupItem(const int iIndex);

    // 好友操作，添加好友直接添加到iIndex分组的末尾
    void addFriendItem(const int iGroupIndex, const FriendInfo& rtFriendInfo);
    void moveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const int iIndex);
    void delFriendItem(const int iGroupIndex, const int iIndex);

    // 发送添加分组
    void sendToAddGroupItem(const QString& rsId, const QString& rsName);
    // 发送重命名分组
    void sendToRenameGroupItem(const QString& rsId, const int iIndex, const QString& rsName);
    // 发送删除分组
    void sendToDelGroupItem(const QString& rsId, const int iGroupIndex);

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
    // 设置控件
    void initUserInofControls(/*const UserInfo& rtMyselfInfo*/);
    void initGroupItemControls(/*const UserInfo& rtMyselfInfo*/);
    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    // 设置SocketClient
    void setSocketClient(void);
    // 获取好友列表
    void sendToGetFriendList(const int iGroupIndex);
    // 向服务器发送id查询好友是否存在
    // 添加好友请求
    void sendToAddFriendRequest(const int iGroupIndex);

    // 解析添加分组信息
    void parseAddGroupItem(const QJsonValue& rtData);
    // 解析删除分组信息
    void parseDelGroupItem(const QJsonValue& rtData);
    // 解析重命名分组信息
    void parseRenameGroupItem(const QJsonValue& rtData);

    // 解析好友列表
    void parseGetFriendList(const QJsonValue& rtData);
    // 解析添加好友状态
    void parseAddFriendStatus(const QJsonValue& rtData);
    // 解析并构造好友信息
    void parseAddFriendInfo(const QJsonValue& rtData, FriendInfo& rtFriendInfo);

    Ui::Friend*   m_ptUi;
    Group*        m_ptGroup;
    SocketClient* m_ptSocketClient;
};

#endif // LINPOP_FRIEND_H
