
#ifndef LINPOP_FRIEND_GROUPITEM_H
#define LINPOP_FRIEND_GROUPITEM_H

#include <common.h>
#include <frienditem.h>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QList>
#include <QEvent>

struct GroupItemUserCount {
    int iTotalCount;            // Group中的人数
    int iOnlineCount;           // 在线人数
};

namespace Ui {
class GroupItem;
}

class GroupItem : public QWidget {
    Q_OBJECT

public:
    explicit GroupItem(const int iGroupIndex, QWidget* ptParent = nullptr);
    ~GroupItem();

    void initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex);

    void addFriendItem(const FriendInfo& rtFriendInfo);
    /**
     * @brief moveFriendItem
     * @param iFriendItemIndex：FirendItem在本个GroupItem中的位置
     * @param iDestGroupItemIndex：要移动至的GroupItem中的位置
     */
    void moveFriendItem(const int iFriendItemIndex, const int iDestGroupItemIndex);
    void moveAllFriendItems(const int iDestGroupItemIndex);
    void delFriendItem(const int iIndex);

    void setGroupItemTextUseCount(void);

    int  getGroupItemIndex(void);
    void setGroupItemIndex(const int iIndex);

    int  getGroupItemCount(void);

    void showPageByClient(const int iIndex);

protected:
    virtual bool eventFilter(QObject* ptWatched, QEvent* ptEvent);

private slots:
    void onAddGroupItem(void);
    void onRenameGroupItem(void);
    void onDelGroupItem(void);
    void onAddFriendItem(void);

    void on_lineEdit_editingFinished();

private:
    Ui::GroupItem*     m_ptUi;
    QList<FriendItem*> m_lptFriendItem;

//    QWidget*           m_ptGroup;           // 指向上一层的Group
    int                m_iGroupIndex;       // 当前GroupItem为Group的第几个

    QList<FriendInfo>  m_ltFriendInfoList;
    GroupItemUserCount m_tGroupUserCount;
};

#endif // LINPOP_FRIEND_GROUPITEM_H
