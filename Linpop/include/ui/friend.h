﻿
#ifndef LINPOP_FRIEND_H
#define LINPOP_FRIEND_H

#include <singleton.h>
#include <common.h>
#include <group.h>
#include <groupitem.h>
#include <addfriend.h>
#include <chat.h>
#include <socketclient.h>

#include <QWidget>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QCloseEvent>

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

class Chat;

class Friend : public QWidget, public Singleton<Friend> {
    Q_OBJECT

public:
    ~Friend();
    // 预初始化
    void preInit(/*const UserInfo& rtMyselfInfo*/);
    void postInit(/*const UserInfo& rtMyselfInfo*/);

    GroupItem* getGroupitemIndex(const int iIndex);
    const int getDefaultGroupitemIndex(void);
    const int getFriendIndexById(const int iGroupIndex, const QString& rsId);

    void showAddFriendUi(const int iIndex);
    // 显示错误或警告信息
    void showTipWindow(const QString& rsTitle, const QString& rsTip,
                       const QString& rsButtonText = QStringLiteral("确定"));

    // 分组操作，添加分组直接添加到分组列表的末尾
    void addGroupItemControls(const int iIndex);
    void renameGroupItemControls(const int iIndex, const QString& rsName);
    void delGroupItemControls(const int iSrcGroupIndex, const int iDestGroupIndex);

    // 好友操作，添加好友直接添加到iIndex分组的末尾
    void addFriendItemControls(const int iIndex, const FriendInfo& rtFriendInfo);
    void moveFriendItemControls(const int iSrcGroupIndex, const int iDestGroupIndex, const int iIndex);
    void delFriendItemControls(const int iGroupIndex, const int iIndex);

    // 聊天界面接口
    void showChatWindow(const int iGroupIndex, const int iIndex);
    void addSendMessageItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                    const QString& rsString, const QString& rsTime);
    void addRecvMessageItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                    const QString& rsString, const QString &rsTime);
    void addHistoryItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                const QString& rsString, const QString& rsTime, const QString &rsId);

protected:
    friend class Singleton<Friend>;
    explicit Friend(QWidget *ptParent = nullptr);
    virtual void closeEvent(QCloseEvent* ptEvent);

private slots:
    // 修改头像按键
    void on_headButton_clicked();
    // 修改名字
    void on_nameEdit_editingFinished();

private:
    // 初始化控件
    void initUserInofControls(/*const UserInfo& rtMyselfInfo*/);
    void initGroupItemControls(/*const UserInfo& rtMyselfInfo*/);
    void initFriendItemListAppend(const int iIndex, const FriendInfo& rtFriendInfo);
    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);
    void updateFriendItemControls(const FriendInfo &rtFriendInfo, const int iFriendIndex);

/** 以下函数均为客户端操作 */
    // 设置SocketClient
    void setSocketClient(void);

public:
    // 添加分组
    void sendToAddGroupItem(void);
    // 重命名分组
    void sendToRenameGroupItem(const int iIndex, const QString& rsName);
    // 删除分组
    void sendToDelGroupItem(const int iSrcGroupIndex, const int iDestGroupIndex);
    // 搜索好友
    void sendToSearchFriendItem(const int iGroupIndex, const QString& rsFriendId);
	// 添加好友请求
	void sendToAddFriendItemRequest(const int iGroupIndex, const QString& rsFriendId);
    // 开启获取好友请求
    void sendToAddFriendItemRequestReady(void);
    // 添加好友
    void sendToAddFriendItem(const int iGroupIndex, const QString& rsFriendId);
    // 移动好友
    void sendToMoveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const QString& rsId, const int iIndex);
    // 删除好友
    void sendToDelFriendItem(const int iGroupIndex, const QString& rsId, const int iIndex);
    // 更新好友状态
    void sendToUpdateFriendStatus(const int iGroupIndex, const QString& rsId, const int iIndex);
    // 发送消息
    void sendToSendMessage(const int iMessageType, const QString& rsString, const int iGroupIndex, const QString& rsId,
                           const int iIndex);
    // 获取历史消息
    void sendToGetHistory(const int iGroupIndex, const QString& rsId, const int iIndex);

private:
    // 获取好友列表
    void sendToGetFriendList(void);
    // 更新昵称
    void sendToUpdateName(void);

    void parseAddGroupItem(const QJsonValue& rtData);
    void parseRenameGroupItem(const QJsonValue& rtData);
    void parseDelGroupItem(const QJsonValue& rtData);
    void parseSearchFriendItem(const QJsonValue& rtData);
	void parseAddFriendItemSendRequest(const QJsonValue& rtData);
    void parseAddFriendItem(const QJsonValue& rtData);
    void parseMoveFriendItem(const QJsonValue& rtData);
    void parseDelFriendItem(const QJsonValue& rtData);
    void parseGetFriendList(const QJsonValue& rtData);
    void parseUpdateFriendStatus(const QJsonValue &rtData);
    void parseUpdateName(const QJsonValue &rtData);
    void parseSendMessage(const QJsonValue &rtData);
    void parseRecvMessage(const QJsonValue &rtData);
    void parseGetHistory(const QJsonValue &rtData);
    // 分组好友信息JSON解析
    void parseGroupFriendInfo(const QJsonValue& rtData);
    // 好友信息JSON解析
    void parseFriendInfo(const QJsonValue& rtData, FriendInfo& rtFriendInfo);
    // 接收消息JSON解析
    void parseGroupRecvMessage(const QJsonValue& rtData);
    // 历史消息JSON解析
    void parseGroupHistory(const QJsonValue& rtData, const int iGroupIndex, const int iFriendIndex);

private slots:
    // 服务器消息处理
    void onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData);
    // 服务器信息处理
    void onSigStatus(int reType/* const Sst& reType */);
    // TCP防粘连延时
    void onTimeout(void);

private:
    Ui::Friend* m_ptUi;
    Group*      m_ptGroup;
    AddFriend*  m_ptAddFriend;
    QMap<Chat*, FriendPosition> m_mpChatMap;      // iGroupIndex, iFriendIndex, 聊天框

    SocketClient* m_ptSocketClient;
};

#endif // LINPOP_FRIEND_H
