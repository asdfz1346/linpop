
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

#ifndef _USE_SQL
#include <QTime>

static QString nameLib[18] = {
    "adsa", "sss", "ddd", "fff", "ggg", "qqq",
    "www", "esa", "rrr", "ddsa", "zzz", "xxx",
    "ccc", "vvv" "bbb", "ttt", "gdsa", "oo",
};

static QString ipLib[18] = {
    "7.124.4.15", "192.168.36.55", "158.6.9.5",
    "7.124.5.25", "192.111.26.5", "151.5.19.5",
    "7.124.6.35", "192.101.16.45", "14.7.29.5",
    "7.124.7.45", "192.8.76.25", "16.8.39.5",
    "7.124.8.55", "192.9.66.5", "182.9.49.5",
    "7.124.9.65", "192.7.6.15", "142.10.59.5",
};
#endif

GroupItem::GroupItem(const int iGroupIndex, QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::GroupItem) {
    m_ptUi->setupUi(this);

    m_tGroupUserCount = { 0 };
    m_iGroupIndex = iGroupIndex;

    setGroupItemTextUseCount();
    m_ptUi->conLayout->setVisible(false);       // 默认为关闭分组的状态
    m_ptUi->itemLayout->installEventFilter(this);
}

GroupItem::~GroupItem() {
    delete m_ptUi;
}
#if 0
void GroupItem::pullFriendItemList(/*const UserInfo& rtMyselfInfo,*/) {
    m_ltFriendInfoList.clear();
#ifdef _USE_SQL
    getFriendListByGroupId(g_tMyselfInfo.sId, m_iGroupIndex, m_ltFriendInfoList);
#else
    QTime tTime;
    tTime= QTime::currentTime();
    qsrand(tTime.msec() + tTime.second() * 1000);

    // 随机产生好友数目
    int iCount  = qrand() % 15 + 1;
    for (int i = 0; i < iCount; ++i) {
        int id       = (qrand() % 90) * 20000 + (qrand() % 90) * 200 + (qrand() % 90) * 2 + 100000;
        int libIndex = qrand() % 16;
        int iconIdex = qrand() % 4 + 1;
        int off      = qrand() % 3;
        FriendInfo usr = { (bool)off, m_iGroupIndex, QString::number(id),
                           (off) ? ipLib[libIndex] : "",            // 离线不显示IP
                           nameLib[libIndex], QString(":/head/%1.png").arg(iconIdex)};
        m_ltFriendInfoList.append(usr);
    }
#endif
}

void GroupItem::pushFriendItemList(/*const UserInfo& rtMyselfInfo,*/) {

}
#endif
void GroupItem::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
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
    QString tNameText = QString("%1 (%2/%3)").arg(g_lsGroupTextList.at(m_iGroupIndex),
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

        QMenu* ptMenu = new QMenu(this);
        ptMenu->addAction(QStringLiteral("添加分组"), this, &GroupItem::onAddGroupItem);
        ptMenu->addAction(QStringLiteral("重命名"), this, &GroupItem::onRenameGroupItem);
        if (m_iGroupIndex) {
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
    m_ptUi->lineEdit->setText(g_lsGroupTextList.at(m_iGroupIndex));
    m_ptUi->stackedWidget->setCurrentIndex(1);
    m_ptUi->lineEdit->setFocus();
    // 服务器事件放在Edit处理事件中
}

void GroupItem::onDelGroupItem(void) {

    if (0 == m_iGroupIndex) {
        QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除分组"), QStringLiteral("默认分组无法删除！"));
        tBox.setStandardButtons(QMessageBox::Ok);
        tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
        tBox.exec();
        return ;
    }

    QString sTips = QStringLiteral("删除此分组后，系统会将好友移动至\n默认分组\"");
    sTips.append(g_lsGroupTextList.at(0));
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
    Friend::getInstance()->sendToDelGroupItem(m_iGroupIndex, 0);
}

void GroupItem::onAddFriendItem(void) {
    // 获取待添加好友的信息

    // 显示添加好友界面
}

static bool g_bFlagEnter = false;
void GroupItem::on_lineEdit_editingFinished() {

    if (m_ptUi->lineEdit->hasFocus()) {
        g_bFlagEnter = true;
        Friend::getInstance()->sendToRenameGroupItem(m_iGroupIndex, m_ptUi->lineEdit->text());
        g_bFlagEnter = false;
        Friend::getInstance()->setFocus();
    }
    else if(!g_bFlagEnter) {
        g_lsGroupTextList.replace(m_iGroupIndex, m_ptUi->lineEdit->text());
        setGroupItemTextUseCount();
        m_ptUi->stackedWidget->setCurrentIndex(0);
    }
}
