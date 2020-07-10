
#ifndef LINPOP_FRIEND_GROUP_H
#define LINPOP_FRIEND_GROUP_H

#include <common.h>
#include <groupitem.h>

#include <QWidget>
#include <QString>
#include <QMap>

namespace Ui {
class Group;
}

class Group : public QWidget {
    Q_OBJECT

public:
    explicit Group(QWidget *ptParent = nullptr);
    ~Group();

    void initGroupItemControls(/*const UserInfo& rtMyselfInfo*/);
    void initFriendItemListAppend(const int iIndex, const FriendInfo& rtFriendInfo);
    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    GroupItem* getGroupitemIndex(const int iIndex);

    // 分组操作，添加分组直接添加到分组列表的末尾
    void addGroupItemControls(const int iIndex, const QString& rsName);
    void renameGroupItemControls(const int iIndex, const QString& rsName);
    void delGroupItemControls(const int iSrcGroupIndex, const int iDestGroupIndex);

    // 好友操作，添加好友直接添加到iIndex分组的末尾
    void addFriendItem(const int iIndex, const FriendInfo& rtFriendInfo);
    void moveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const int iIndex);
    void delFriendItem(const int iGroupIndex, const int iIndex);

private:
    int  findDefaultGroupItem(const int iIndex);

    Ui::Group*            m_ptUi;
    QMap<int, GroupItem*> m_mptGroupItem;
};

#endif // LINPOP_FRIEND_GROUP_H
