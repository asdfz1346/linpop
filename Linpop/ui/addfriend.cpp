
#include <addfriend.h>
#include <ui_addfriend.h>
#include <friend.h>

#include <QPixmap>

AddFriend::AddFriend(const int iGroupIndex, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::AddFriend) {
    m_ptUi->setupUi(this);

    this->setWindowFlags(windowFlags() &~Qt::WindowMinMaxButtonsHint);

    m_iGroupIndex = iGroupIndex;
}

AddFriend::~AddFriend() {
    delete m_ptUi;
}

void AddFriend::showStackPage(const int iIndex, const FriendInfo& rtFriendInfo) {
    if (iIndex) {
        // 如果进入，必然是已经找到好友
        m_ptUi->stackedWidget->setCurrentIndex(1);
        m_ptUi->sureButton->setText(QStringLiteral("确 定"));
        m_ptUi->cancelButton->setText(QStringLiteral("返 回"));
        m_ptUi->headLabel->setPixmap(QPixmap(rtFriendInfo.sHead));
        if (!rtFriendInfo.bOnline) {
            m_ptUi->headLabel->setDisabled(true);
        }
        m_ptUi->nameLabel->setText(rtFriendInfo.sName);
        m_ptUi->idLabel->setText(rtFriendInfo.sId);
        m_ptUi->ipLabel->setText(rtFriendInfo.sIp);
    }
    else {
        m_ptUi->stackedWidget->setCurrentIndex(0);
        m_ptUi->idEdit->setText("");
        m_ptUi->sureButton->setText(QStringLiteral("查 找"));
        m_ptUi->cancelButton->setText(QStringLiteral("取 消"));

        Friend::getInstance()->show();
        this->close();
    }
}

void AddFriend::setGroupIndex(const int iIndex) {
    m_iGroupIndex = iIndex;
}

void AddFriend::on_sureButton_clicked() {
    if (m_ptUi->stackedWidget->currentIndex()) {
        // 发送添加好友请求
        Friend::getInstance()->sendToAddFriendItemRequest(m_iGroupIndex, m_ptUi->idLabel->text());
        // 退出添加好友界面显示
        showStackPage(0, { 0 });
    }
    else {
        // 发送搜索好友请求
        Friend::getInstance()->sendToSearchFriendItem(m_iGroupIndex, m_ptUi->idEdit->text());
    }
}

void AddFriend::on_cancelButton_clicked() {
    if (m_ptUi->stackedWidget->currentIndex()) {
        m_ptUi->stackedWidget->setCurrentIndex(0);
        m_ptUi->sureButton->setText(QStringLiteral("查 找"));
        m_ptUi->cancelButton->setText(QStringLiteral("取 消"));
    }
    else {
        Friend::getInstance()->show();
        this->close();
    }
}
