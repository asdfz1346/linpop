
#ifndef LINPOP_FRIEND_GROUP_H
#define LINPOP_FRIEND_GROUP_H

#include <common.h>
#include <groupitem.h>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QList>

namespace Ui {
class Group;
}

class Group : public QWidget {
    Q_OBJECT

public:
    explicit Group(QWidget *ptParent = nullptr);
    ~Group();

    // Friend的底层接口
    int  pullGroupItemList(/*const UserInfo& rtMyselfInfo*/);
    void pushGroupItemList(/*const UserInfo& rtMyselfInfo*/);

    void pullFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);
    void pushFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    void initGroupItemControls(/*const UserInfo& rtMyselfInfo*/);
    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    void addGroupItem(const QString& rsName = GROUPITEM_NAME_DEFAULT);
    void delGroupItem(const int iIndex);
    void addFriendItem(const int iIndex, const FriendInfo& rtFriendInfo);

    GroupItem* getGroupitemIndex(const int iIndex);

private:
    int  findDefaultGroupItem(const int iIndex);

    Ui::Group*        m_ptUi;
    QList<GroupItem*> m_lptGroupItem;
};

#endif // LINPOP_FRIEND_GROUP_H
