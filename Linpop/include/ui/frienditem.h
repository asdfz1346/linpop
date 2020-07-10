
#ifndef LINPOP_FRIENDITEM_H
#define LINPOP_FRIENDITEM_H

#include <common.h>

#include <QWidget>
#include <QContextMenuEvent>

namespace Ui {
class FriendItem;
}

class FriendItem : public QWidget {
    Q_OBJECT

public:
    explicit FriendItem(const FriendInfo& rtFriendInfo, const int iFriendIndex, QWidget* ptParent = nullptr);
    ~FriendItem();

    void setFriendItemIndex(const int iIndex);

protected:
    virtual void contextMenuEvent(QContextMenuEvent* ptEvent);

private slots:
    void onMoveFriendItem(QAction* ptAct);
    void onDelFriendItem(void);

private:
    Ui::FriendItem* m_ptUi;
    QWidget*        m_ptGroupItem;          // 指向上一层的GroupItem
    int             m_iFriendIndex;         // 当前FriendItem位于上一层Map中的index
};

#endif // LINPOP_FRIENDITEM_H
