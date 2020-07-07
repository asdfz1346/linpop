
#include <friend.h>
#include <ui_friend.h>

Friend::Friend(QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::Friend) {
    m_ptUi->setupUi(this);
    m_ptGroup = new Group(this);
    m_ptUi->friendWidget->layout()->addWidget(m_ptGroup);
}

Friend::~Friend() {
    delete m_ptGroup;
    delete m_ptUi;
}

void Friend::preInit(/*const UserInfo& rtMyselfInfo*/) {
    // 设置个人信息
    initUserInofControls();
    // 获取并设置分组信息
    int iRet = pullGroupItemList();
    initGroupItemControls();
    // 获取并设置每个分组的好友信息
    for (int i = 0; i < iRet; ++i) {
        pullFriendItemList(i);
        initFriendItemControls(i);
    }
}

void Friend::postInit(/*const UserInfo& rtMyselfInfo*/) {
    /* Nothing */
}

void Friend::pushUserInfo(/*const UserInfo& rtMyselfInfo*/) {

}

int  Friend::pullGroupItemList(/*const UserInfo& rtMyselfInfo*/) {
    return m_ptGroup->pullGroupItemList();
}

void Friend::pushGroupItemList(/*const UserInfo& rtMyselfInfo*/) {
    return m_ptGroup->pushGroupItemList();
}

void Friend::pullFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_ptGroup->pullFriendItemList(iIndex);
}

void Friend::pushFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex){
    m_ptGroup->pushFriendItemList(iIndex);
}

GroupItem* Friend::getGroupitemIndex(const int iIndex) {
    return m_ptGroup->getGroupitemIndex(iIndex);
}

void Friend::on_iconButton_clicked() {

}

void Friend::initUserInofControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
    m_ptUi->idLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->iconButton->setStyleSheet(QString("border-image: url(")
                                      .append(g_tMyselfInfo.sIcon + ")"));
}

void Friend::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    // GroupItemList存放在Group中
    m_ptGroup->initGroupItemControls();
}

void Friend::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    // GroupFriendList存放在GroupItem中
    m_ptGroup->initFriendItemControls(iIndex);
}
