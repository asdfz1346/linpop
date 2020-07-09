
#include <addfriend.h>
#include <ui_addfriend.h>

#include <QPixmap>

AddFriend::AddFriend(const FriendInfo& rtFriendInfo, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::AddFriend) {
    m_ptUi->setupUi(this);

    m_ptUi->headLabel->setPixmap(QPixmap(rtFriendInfo.sHead));
    if (!rtFriendInfo.bOnline) {
        m_ptUi->headLabel->setDisabled(true);
    }
    m_ptUi->nameLabel->setText(rtFriendInfo.sName);
    m_ptUi->idLabel->setText(rtFriendInfo.sId);
    m_ptUi->ipLabel->setText(rtFriendInfo.sIp);
}

AddFriend::~AddFriend() {
    delete m_ptUi;
}
