
#include <friend.h>
#include <ui_friend.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>

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
    setSocketClient();
    // 设置个人信息控件
    initUserInofControls();
    // 设置分组控件
    initGroupItemControls();

    // 添加好友请求示例（暂时未实现添加好友功能）
    sendToAddFriendRequest(0);
#if 0
    // 获取并设置每个分组的好友信息
    int iLength = g_lsGroupTextList.length();
    for (int i = 0; i < iLength; ++i) {
        // 构造
        sendIdAndGroupIndex(i);
        // 初始化FriendItems控件放在
//        initFriendItemControls(i);
    }
#endif
}

void Friend::postInit(/*const UserInfo& rtMyselfInfo*/) {
    /* Nothing */
}

GroupItem* Friend::getGroupitemIndex(const int iIndex) {
    return m_ptGroup->getGroupitemIndex(iIndex);
}

void Friend::addGroupItem(const QString& rsName) {
    m_ptGroup->addGroupItem(rsName);
}

void Friend::renameGroupItem(const int iIndex, const QString& rsName) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << iIndex << rsName;
#endif
    m_ptGroup->renameGroupItem(iIndex, rsName);
}

void Friend::delGroupItem(const int iIndex) {
    m_ptGroup->delGroupItem(iIndex);
}

void Friend::addFriendItem(const int iGroupIndex, const FriendInfo& rtFriendInfo) {
    m_ptGroup->addFriendItem(iGroupIndex, rtFriendInfo);
}

void Friend::moveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex,
                            const int iIndex) {
    m_ptGroup->moveFriendItem(iSrcGroupIndex, iDestGroupIndex, iIndex);
}

void Friend::delFriendItem(const int iGroupIndex, const int iIndex) {
    m_ptGroup->delFriendItem(iGroupIndex, iIndex);
}

void Friend::sendToAddGroupItem(const QString& rsId, const QString& rsName) {
    QJsonObject tData;
    tData.insert("Id", rsId);
    tData.insert("GroupName", rsName);

#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << tData;
#endif

    m_ptSocketClient->onSendMessage(SMT_ADDGROUP, tData);
}

void Friend::sendToRenameGroupItem(const QString& rsId, const int iIndex, const QString& rsName){
    QJsonObject tData;
    tData.insert("Id", rsId);
    tData.insert("GroupIndex", iIndex);
    tData.insert("GroupName", rsName);

    m_ptSocketClient->onSendMessage(SMT_RENAMEGROUP, tData);
}

void Friend::sendToDelGroupItem(const QString& rsId, const int iGroupIndex) {
    QJsonObject tData;
    tData.insert("Id", rsId);
    tData.insert("GroupIndex", iGroupIndex);

    m_ptSocketClient->onSendMessage(SMT_DELGROUP, tData);
}

void Friend::on_headButton_clicked() {
    QString sPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择头像"),
                                                 ".", "Image Files(*.png)");
    if (sPath.length()) {
        QImage tImage;
        if (tImage.load(sPath)) {
            // 更新全局用户成员
            g_tMyselfInfo.sHead = sPath;
            m_ptUi->headButton->setStyleSheet(QString("border-image: url(")
                                              .append(g_tMyselfInfo.sHead + ")"));
        }
    }
}

void Friend::onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData) {
    switch (reType) {
        case SMT_GETFRIEND: {
            // 获取好友列表
            break;
        }
        case SMT_ADDFRIEND: {
            // 解析添加好友状态
            parseAddFriendStatus(rtData);
            break;
        }
        case SMT_ADDGROUP: {
            // 解析添加分组状态
            parseAddGroupItem(rtData);
            break;
        }
        case SMT_DELGROUP: {
            // 解析删除分组状态
            parseDelGroupItem(rtData);
            break;
        }
        case SMT_RENAMEGROUP: {
            // 解析重命名分组状态
            parseRenameGroupItem(rtData);
            break;
        }
    }
}

void Friend::onSigStatus(int reType/* const Sst& reType */) {
    switch (reType) {
        case SST_GETFRIEND_SUCCESS: {
            // 获取好友列表
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
        case SST_ADDFRIEND_SUCCESS: {
            /* Nothing */
            break;
        }
    }
}

void Friend::initUserInofControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
    m_ptUi->idLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->headButton->setStyleSheet(QString("border-image: url(")
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

    connect(m_ptSocketClient, SIGNAL(sigMessage(int,QJsonValue)), this, SLOT(onSigMessage(int,QJsonValue)));
    connect(m_ptSocketClient, SIGNAL(sigStatus(int)), this, SLOT(onSigStatus(int)));
}

void Friend::sendToGetFriendList(const int iGroupIndex) {
    QJsonObject tData;
    tData.insert("Id", g_tMyselfInfo.sId);
    tData.insert("GroupIndex", iGroupIndex);

    m_ptSocketClient->onSendMessage(SMT_GETFRIEND, tData);
}

void Friend::sendToAddFriendRequest(const int iGroupIndex) {
    QJsonObject tData;
    tData.insert("Id", "5");
    tData.insert("GroupIndex", iGroupIndex);

    m_ptSocketClient->onSendMessage(SMT_ADDFRIEND, tData);
}

void Friend::parseAddGroupItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_ADDGROUP_SUCCESS == iStatus) {
            // 添加分组
            addGroupItem();
            Q_EMIT m_ptSocketClient->sigStatus(SST_ADDGROUP_SUCCESS);
            return ;
        }
        Q_EMIT m_ptSocketClient->sigStatus(SST_ADDFRIEND_FAILED);
        return ;
    }
}

void Friend::parseDelGroupItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_DELGROUP_SUCCESS == iStatus) {
            // 删除分组
            delGroupItem(tObj.value("GroupIndex").toInt());
            Q_EMIT m_ptSocketClient->sigStatus(SST_DELGROUP_SUCCESS);
            return ;
        }
        Q_EMIT m_ptSocketClient->sigStatus(SST_DELGROUP_FAILED);
        return ;
    }
}

void Friend::parseRenameGroupItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_RENAMEGROUP_SUCCESS == iStatus) {
            // 重命名
            renameGroupItem(tObj.value("GroupIndex").toInt(), tObj.value("GroupName").toString());
            Q_EMIT m_ptSocketClient->sigStatus(SST_RENAMEGROUP_SUCCESS);
            return ;
        }
        Q_EMIT m_ptSocketClient->sigStatus(SST_RENAMEGROUP_FAILED);
        return ;
    }
}

#if 0
void Friend::sendToGetAddFriendInfo() {

}
#endif

void Friend::parseGetFriendList(const QJsonValue& rtData) {
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        QJsonArray  tArr;
        if (tObj.value("Friend").isArray()) {
            tArr = tObj.value("Friend").toArray();
        }

        int iStatus = tObj.value("Status").toInt();
        int iIndex  = tObj.value("GroupIndex").toInt();
        if ((SST_GETFRIEND_SUCCESS == iStatus) && tArr.size()) {
            // 填充FriendItem，
//            Q_EMIT sigStatus(SST_GETFRIEND_SUCCESS);
            return ;
        }
//        Q_EMIT sigStatus(SST_GETFRIEND_FAILED);
        return ;
    }
}

void Friend::parseAddFriendStatus(const QJsonValue& rtData) {
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_ADDFRIEND_SUCCESS == iStatus) {
            int iIndex      = tObj.value("GroupIndex").toInt();
            QJsonValue tVal = tObj.value("Friend");
            // 解析出FriendInfo
            FriendInfo tUserInfo = { 0 };
            tUserInfo.iGroup = iIndex;
            parseAddFriendInfo(tVal, tUserInfo);
            // 添加好友变量并设置界面
            addFriendItem(iIndex, tUserInfo);
            Q_EMIT m_ptSocketClient->sigStatus(SST_ADDFRIEND_SUCCESS);
            return ;
        }
        Q_EMIT m_ptSocketClient->sigStatus(SST_ADDFRIEND_FAILED);
        return ;
    }
}

void Friend::parseAddFriendInfo(const QJsonValue& rtData, FriendInfo& rtFriendInfo) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        // 解析出FriendInfo
        rtFriendInfo.bOnline = (bool)tObj.value("Online").toInt();
        rtFriendInfo.sHead   = tObj.value("Head").toString();
        rtFriendInfo.sId     = tObj.value("Id").toString();
        rtFriendInfo.sIp     = tObj.value("Ip").toString();
        rtFriendInfo.sName   = tObj.value("Name").toString();
    }
}
