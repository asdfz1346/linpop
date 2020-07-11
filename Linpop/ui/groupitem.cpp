
#include <groupitem.h>
#include <group.h>
#include <frienditem.h>
#include <ui_groupitem.h>
#include <friend.h>

#include <string>

#include <QMenu>
#include <QAction>
#include <QLayoutItem>
#include <QMessageBox>

GroupItem::GroupItem(const int iGroupIndex, QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::GroupItem) {
    m_ptUi->setupUi(this);

    m_tGroupUserCount = { 0 };
    m_iGroupIndex = iGroupIndex;
    m_ltFriendInfoList.clear();

    setGroupItemTextUseCount();
    m_ptUi->conLayout->setVisible(false);       // 默认为关闭分组的状态
    m_ptUi->itemLayout->installEventFilter(this);
}

GroupItem::~GroupItem() {
    delete m_ptUi;
}

void GroupItem::initFriendItemListAppend(const FriendInfo& rtFriendInfo) {
    m_ltFriendInfoList.append(rtFriendInfo);
}

void GroupItem::initFriendItemControls(void) {
    for (int i = 0; i < m_ltFriendInfoList.length(); ++i) {
        FriendItem* ptItem = new FriendItem(m_ltFriendInfoList.at(i), i, this);
        m_ptUi->conLayout->layout()->addWidget(ptItem);

        ++(m_tGroupUserCount.iTotalCount);
        if (m_ltFriendInfoList.at(i).bOnline) {
            ++(m_tGroupUserCount.iOnlineCount);
        }
        m_lptFriendItem.append(ptItem);
    }

    setGroupItemTextUseCount();
    // 从此处开始，m_lptFriendItem、m_tGroupUserCount和m_ltFriendInfoList的设置都由客户端同步完成
}

void GroupItem::addFriendItem(const FriendInfo& rtFriendInfo) {
    FriendItem* ptItem = new FriendItem(rtFriendInfo, m_ltFriendInfoList.length(), this);
    m_ptUi->conLayout->layout()->addWidget(ptItem);

    ++(m_tGroupUserCount.iTotalCount);
    if (rtFriendInfo.bOnline) {
        ++(m_tGroupUserCount.iOnlineCount);
    }
    m_lptFriendItem.append(ptItem);
    m_ltFriendInfoList.append(rtFriendInfo);

    setGroupItemTextUseCount();
}

void GroupItem::moveFriendItem(const int iFriendItemIndex, const int iDestGroupItemIndex) {
    Friend*     ptFriend = Friend::getInstance();
    GroupItem*  ptItem   = ptFriend->getGroupitemIndex(iDestGroupItemIndex);

    ptItem->addFriendItem(m_ltFriendInfoList.at(iFriendItemIndex));
    delFriendItem(iFriendItemIndex);
}

void GroupItem::moveAllFriendItems(const int iDestGroupItemIndex) {
    int iCount = m_tGroupUserCount.iTotalCount;     // 不能直接使用成员变量，因为在delete过程中会发生变化
    for ( ; iCount; --iCount) {
        moveFriendItem(0, iDestGroupItemIndex);
    }
}

void GroupItem::delFriendItem(const int iIndex) {
    if (0 > iIndex)
        return ;

    m_lptFriendItem.at(iIndex)->setVisible(false);
    m_ptUi->conLayout->layout()->removeWidget(m_lptFriendItem.at(iIndex));

    --(m_tGroupUserCount.iTotalCount);
    if (m_ltFriendInfoList.at(iIndex).bOnline) {
        --(m_tGroupUserCount.iOnlineCount);
    }
    m_lptFriendItem.removeAt(iIndex);
    m_ltFriendInfoList.removeAt(iIndex);

    setGroupItemTextUseCount();

    // 需要更新其他Friend的index
    for (int i = 0; i < m_lptFriendItem.length(); ++i) {
        m_lptFriendItem.at(i)->setFriendItemIndex(i);
    }
}

void GroupItem::setGroupItemTextUseCount(void) {
    QString tNameText = QString("%1 (%2/%3)").arg(g_msGroupTextMap[m_iGroupIndex],
                                                  QString::number(m_tGroupUserCount.iOnlineCount),
                                                  QString::number(m_tGroupUserCount.iTotalCount));
    m_ptUi->textLabel->setText(tNameText);
}

int  GroupItem::getGroupItemIndex(void) {
    return m_iGroupIndex;
}

void GroupItem::setGroupItemIndex(const int iIndex) {
    m_iGroupIndex = iIndex;
}

int  GroupItem::getGroupItemCount(void) {
    return m_tGroupUserCount.iTotalCount;
}

void GroupItem::showStackPage(const int iIndex) {
    m_ptUi->stackedWidget->setCurrentIndex(iIndex);
}

bool GroupItem::eventFilter(QObject* ptWatched, QEvent* ptEvent) {
    if (QEvent::MouseButtonPress != ptEvent->type()) {
        return false;
    }
    if (m_ptUi->itemLayout != ptWatched) {
        return false;
    }

    QMouseEvent* ptMouseEvent = static_cast<QMouseEvent*>(ptEvent);
    if (Qt::LeftButton == ptMouseEvent->button()) {
        m_ptUi->conLayout->setVisible(!m_ptUi->conLayout->isVisible());
        if (m_ptUi->conLayout->isVisible()) {
            m_ptUi->iconLabel->setPixmap(QPixmap(":/friend/arrow_d.png"));
        }
        else {
            m_ptUi->iconLabel->setPixmap(QPixmap(":/friend/arrow_r.png"));
        }
        return true;
    }
    else if (Qt::RightButton == ptMouseEvent->button()) {
//        m_ptUi->itemLayout->setStyleSheet("itemLayout {\n\tbackground-color: rgb(215, 215, 215);\n}");
        // 获取存储待删除分组好友的目标分组
        int iDestGroupIndex = Friend::getInstance()->getDefaultGroupitemIndex();

        QMenu* ptMenu = new QMenu(this);
        ptMenu->addAction(QStringLiteral("添加分组"), this, &GroupItem::onAddGroupItem);
        ptMenu->addAction(QStringLiteral("重命名"), this, &GroupItem::onRenameGroupItem);
        if (iDestGroupIndex != m_iGroupIndex) {
            ptMenu->addAction(QStringLiteral("删除此分组"), this, &GroupItem::onDelGroupItem);
        }
        ptMenu->addAction(QStringLiteral("向此分组添加好友"), this, &GroupItem::onAddFriendItem);
        ptMenu->exec(QCursor::pos());

        m_ptUi->itemLayout->setStyleSheet("itemLayout {\n\tbackground-color: rgba(0, 0, 0, 0);\n}");
        ptMenu->deleteLater();
        return true;
    }
    return false;
}

void GroupItem::onAddGroupItem(void) {
    Friend::getInstance()->sendToAddGroupItem();
}

void GroupItem::onRenameGroupItem(void) {
    m_ptUi->lineEdit->setText(g_msGroupTextMap[m_iGroupIndex]);
    m_ptUi->stackedWidget->setCurrentIndex(1);
    m_ptUi->lineEdit->setFocus();
    // 服务器事件放在Edit处理事件中
}

void GroupItem::onDelGroupItem(void) {
    // 获取存储待删除分组好友的目标分组
    Friend* ptFriend = Friend::getInstance();
    int iDestGroupIndex = ptFriend->getDefaultGroupitemIndex();
    // 不允许删除默认分组
    if (iDestGroupIndex == m_iGroupIndex) {
        QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除分组"), QStringLiteral("默认分组无法删除！"));
        tBox.setStandardButtons(QMessageBox::Ok);
        tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
        tBox.exec();
        return ;
    }

    QString sTips = QStringLiteral("删除此分组后，系统会将好友移动至\n默认分组\"");
    sTips.append(g_msGroupTextMap.first());
    sTips.append(QStringLiteral("\"中"));
    QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除分组"), sTips);
    tBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    tBox.setButtonText(QMessageBox::Yes, QString(QStringLiteral("确定")));
    tBox.setButtonText(QMessageBox::No,  QString(QStringLiteral("取消")));
    int iRet = tBox.exec();
    if (QMessageBox::No == iRet) {
        return ;
    }

    // 发送删除分组请求
    ptFriend->sendToDelGroupItem(m_iGroupIndex, iDestGroupIndex);
}

void GroupItem::onAddFriendItem(void) {
    // 显示搜索好友界面
    Friend::getInstance()->showAddFriendUi(m_iGroupIndex);
}

void GroupItem::on_lineEdit_editingFinished() {
    // 为什么两次处理不一样？
    if (m_ptUi->lineEdit->hasFocus()) {
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__;
#endif
        Friend::getInstance()->setFocus();
    }
    else {
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__;
#endif
        Friend::getInstance()->sendToRenameGroupItem(m_iGroupIndex, m_ptUi->lineEdit->text());
        g_msGroupTextMap[m_iGroupIndex] = m_ptUi->lineEdit->text();
        setGroupItemTextUseCount();
        m_ptUi->stackedWidget->setCurrentIndex(0);
    }
}
