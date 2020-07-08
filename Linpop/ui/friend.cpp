
#include <friend.h>
#include <ui_friend.h>

#include <QFileDialog>
#include <QMessageBox>

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
    // 在设置SocketClient之前，它的所有信号必须全部断开！

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
    QString sPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择头像"), ".", "Image Files(*.png)");
    if (sPath.length()) {
        QImage tImage;
        if (tImage.load(sPath)) {
            // 更新全局用户成员
            g_tMyselfInfo.sHead = sPath;
            m_ptUi->iconButton->setStyleSheet(QString("border-image: url(")
                                              .append(g_tMyselfInfo.sHead + ")"));
        }
    }
}

void Friend::onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData) {
    switch (reType) {
        case SMT_GETFRIEND: {
            // 拉取好友信息
            break;
        }
        case SMT_ADDFRIEND: {
            QMessageBox tBox(QMessageBox::Warning, QStringLiteral("错误"),
                             QStringLiteral("初始化失败！\n请重试！"));
            tBox.setStandardButtons(QMessageBox::Ok);
            tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
            tBox.exec();
            break;
        }
    }
}

void Friend::onSigStatus(int reType/* const Sst& reType */) {
    switch (reType) {
        case SST_GETFRIEND_SUCCESS: {
            // 拉取好友信息
            break;
        }
        case SST_GETFRIEND_FAILED: {
            QMessageBox tBox(QMessageBox::Warning, QStringLiteral("错误"),
                             QStringLiteral("初始化失败！\n请重试！"));
            tBox.setStandardButtons(QMessageBox::Ok);
            tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
            tBox.exec();
            break;
        }
    }
}

void Friend::initUserInofControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
    m_ptUi->idLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->iconButton->setStyleSheet(QString("border-image: url(")
                                      .append(g_tMyselfInfo.sHead + ")"));
}

void Friend::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    // GroupItemList存放在Group中
    m_ptGroup->initGroupItemControls();
}

void Friend::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    // GroupFriendList存放在GroupItem中
    m_ptGroup->initFriendItemControls(iIndex);
}

void Friend::setSocketClient(void) {
    m_ptSocketClient = SocketClient::getInstance();
    // 连接由Loggin完成
//    m_ptSocketClient->connectToServer(g_tServerIpAddr);

    connect(m_ptSocketClient, SIGNAL(sigMessage(int,QJsonValue)), this, SLOT(onSigMessage(int,QJsonValue)));
    connect(m_ptSocketClient, SIGNAL(sigStatus(int)), this, SLOT(onSigStatus(int)));
}
