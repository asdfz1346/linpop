
#include <chat.h>
#include <ui_chat.h>

Chat::Chat(const FriendPosition &rtPosition, const FriendInfo &rtFriendInfo, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::Chat) {
    m_ptUi->setupUi(this);

    m_tPosition.iFriendIndex = rtPosition.iFriendIndex;
    m_tPosition.iGroupIndex  = rtPosition.iGroupIndex;
    // 更新个人信息
    m_ptUi->myHeadLabel->setPixmap(QPixmap(g_tMyselfInfo.sHead));
    m_ptUi->myIdLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->myIpLabel->setText(g_tMyselfInfo.sIp);
    m_ptUi->myNameLabel->setText(g_tMyselfInfo.sName);
    // 更新好友信息
    m_ptUi->headLabel->setPixmap(QPixmap(rtFriendInfo.sHead));
    m_ptUi->idLabel->setText(rtFriendInfo.sId);
    m_ptUi->ipLabel->setText(rtFriendInfo.sIp);
    m_ptUi->nameLabel->setText(rtFriendInfo.sName);
}

Chat::~Chat() {
    delete m_ptUi;
}

void Chat::on_sendButton_clicked() {
}
