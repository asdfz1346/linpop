
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
    void on_iconButton_clicked();
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

    Ui::Friend*   m_ptUi;
    Group*        m_ptGroup;
    SocketClient* m_ptSocketClient;
};

#endif // LINPOP_FRIEND_H
