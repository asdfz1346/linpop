﻿
#include <frienditem.h>
#include <ui_frienditem.h>
#include <groupitem.h>
#include <friend.h>

#include <QPixmap>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QObject>

FriendItem::FriendItem(const FriendInfo& rtFriendInfo, const int iFriendIndex, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::FriendItem) {
    m_ptUi->setupUi(this);

    m_ptUi->headLabel->setPixmap(QPixmap(rtFriendInfo.sHead));
    if (!rtFriendInfo.bOnline) {
        m_ptUi->headLabel->setDisabled(true);
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
    int iGroupItemIndex = ((GroupItem*)m_ptGroupItem)->getGroupItemIndex();

    QMap<int, QString>::iterator tIter = g_msGroupTextMap.begin();
//    while (tIter != g_msGroupTextMap.end()) {
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << tIter.key() << tIter.value();
#endif
//        if (iGroupItemIndex == tIter.key()) {
//            continue;
//        }
//        QAction* ptAct = ptMoveMenu->addAction(tIter.value());
//        ptAct->setData(tIter.key());
//        ++tIter;
//    }
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
    int iSrcGroupIndex = ((GroupItem*)m_ptGroupItem)->getGroupItemIndex();

    Friend::getInstance()->sendToMoveFriendItem(iSrcGroupIndex, ptAct->data().toInt(), m_ptUi->idLabel->text(), m_iFriendIndex);
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

    int iGroupIndex = ((GroupItem*)m_ptGroupItem)->getGroupItemIndex();
    Friend::getInstance()->sendToDelFriendItem(iGroupIndex, m_ptUi->idLabel->text(), m_iFriendIndex);
}
