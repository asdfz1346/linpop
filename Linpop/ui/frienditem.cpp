﻿
#include <frienditem.h>
#include <ui_frienditem.h>
#include <groupitem.h>

#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QObject>

FriendItem::FriendItem(const FriendInfo& rtFriendInfo, const int iFriendIndex, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::FriendItem) {
    m_ptUi->setupUi(this);

    m_ptUi->iconLabel->setPixmap(QPixmap(rtFriendInfo.sHead));
    if (!rtFriendInfo.bOnline) {
        m_ptUi->iconLabel->setDisabled(true);
    }
    m_ptUi->nameLabel->setText(rtFriendInfo.sName);
    m_ptUi->idLabel->setText(rtFriendInfo.sId);
    m_ptUi->ipLabel->setText(rtFriendInfo.sIp);

    m_ptGroupItem  = ptParent;
    m_iFriendIndex = iFriendIndex;
}

FriendItem::~FriendItem() {
    delete m_ptUi;
}

void FriendItem::setFriendItemIndex(const int iIndex) {
    m_iFriendIndex = iIndex;
}

void FriendItem::contextMenuEvent(QContextMenuEvent* ptEvent) {
    Q_UNUSED(ptEvent);
    QMenu* ptMenu      = new QMenu(this);
    QMenu* ptMoveMenu  = new QMenu(this);

    QAction* ptMoveAct = ptMenu->addAction(QStringLiteral("移动联系人至"));
    int iGruopItemIndex = ((GroupItem*)m_ptGroupItem)->getGroupItemIndex();
    for (int i = 0; i < g_lsGroupTextList.length(); ++i) {
        if (iGruopItemIndex == i) {
            continue;
        }
        QAction* ptAct = ptMoveMenu->addAction(g_lsGroupTextList.at(i));
        ptAct->setData(i);
    }
    QObject::connect(ptMoveMenu, SIGNAL(triggered(QAction*)), this, SLOT(onMoveFriendItem(QAction*)));
    ptMoveAct->setMenu(ptMoveMenu);

    ptMenu->addMenu(ptMoveMenu);
    ptMenu->addAction(QStringLiteral("删除好友"), this, &FriendItem::onDelFriendItem);
    ptMenu->exec(QCursor::pos());
    m_ptUi->FriendLayout->setStyleSheet("FriendLayout {\n\tbackground-color: rgba(0, 0, 0, 0);\n}");

    ptMoveMenu->deleteLater();
    ptMenu->deleteLater();
}

void FriendItem::onMoveFriendItem(QAction* ptAct) {
    ((GroupItem*)m_ptGroupItem)->moveFriendItem(m_iFriendIndex, ptAct->data().toInt());
}

void FriendItem::onDelFriendItem(void) {
    /* 提示消息 */
    QString sTips = QStringLiteral("是否删除ID号\n为\"");
    sTips.append(m_ptUi->idLabel->text());
    sTips.append(QStringLiteral("\"的好友？"));

    QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除好友"), sTips);
    tBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    tBox.setButtonText(QMessageBox::Yes, QString(QStringLiteral("确定")));
    tBox.setButtonText(QMessageBox::No, QString(QStringLiteral("取消")));
    int iRet = tBox.exec();
    if (QMessageBox::No == iRet) {
        return ;
    }

    ((GroupItem*)m_ptGroupItem)->delFriendItem(m_iFriendIndex);
}
