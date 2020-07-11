
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
    m_ptAddFriend = nullptr;
}

Friend::~Friend() {
    if (m_ptAddFriend)
        delete m_ptAddFriend;
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

    QMap<int, QString>::iterator tIter = g_msGroupTextMap.begin();
    while (tIter != g_msGroupTextMap.end()) {
        sendToGetFriendList(tIter.key());
        ++tIter;
    }

    // 添加好友请求示例（暂时未实现添加好友功能）
    //sendToAddFriendItem(0, "1");
}

void Friend::postInit(/*const UserInfo& rtMyselfInfo*/) {
    // 告诉服务器可以发送之前暂存的添加好友信息了
    sendToAddFriendItemRequestReady();
}

GroupItem* Friend::getGroupitemIndex(const int iIndex) {
    return m_ptGroup->getGroupitemIndex(iIndex);
}

const int Friend::getDefaultGroupitemIndex(void) {
    return m_ptGroup->getDefaultGroupitemIndex();
}

void Friend::showAddFriendUi(const int iIndex) {
    if (nullptr == m_ptAddFriend) {
        m_ptAddFriend = new AddFriend(iIndex);
    }
    else {
        m_ptAddFriend->setGroupIndex(iIndex);
    }
    m_ptAddFriend->show();
    this->hide();
}

void Friend::addGroupItemControls(const int iIndex) {
    m_ptGroup->addGroupItemControls(iIndex, GROUPITEM_NAME_DEFAULT);
}

void Friend::renameGroupItemControls(const int iIndex, const QString& rsName) {
    m_ptGroup->renameGroupItemControls(iIndex, rsName);
}

void Friend::delGroupItemControls(const int iSrcGroupIndex, const int iDestGroupIndex) {
    m_ptGroup->delGroupItemControls(iSrcGroupIndex, iDestGroupIndex);
}

void Friend::addFriendItemControls(const int iIndex, const FriendInfo& rtFriendInfo) {
    m_ptGroup->addFriendItem(iIndex, rtFriendInfo);
}

void Friend::moveFriendItemControls(const int iSrcGroupIndex, const int iDestGroupIndex,
                                    const int iIndex) {
    m_ptGroup->moveFriendItem(iSrcGroupIndex, iDestGroupIndex, iIndex);
}

void Friend::delFriendItemControls(const int iGroupIndex, const int iIndex) {
    m_ptGroup->delFriendItem(iGroupIndex, iIndex);
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

void Friend::initUserInofControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
    m_ptUi->idLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->headButton->setStyleSheet(QString("border-image: url(")
                                      .append(g_tMyselfInfo.sHead + ")"));
}

void Friend::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptGroup->initGroupItemControls();
}

void Friend::initFriendItemListAppend(const int iIndex, const FriendInfo& rtFriendInfo) {
    m_ptGroup->initFriendItemListAppend(iIndex, rtFriendInfo);
}

void Friend::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_ptGroup->initFriendItemControls(iIndex);
}

void Friend::setSocketClient(void) {
    m_ptSocketClient = SocketClient::getInstance();
    // 连接由Loggin完成

    connect(m_ptSocketClient, SIGNAL(sigMessage(int,QJsonValue)), this, SLOT(onSigMessage(int,QJsonValue)));
    connect(m_ptSocketClient, SIGNAL(sigStatus(int)), this, SLOT(onSigStatus(int)));
}

void Friend::sendToAddGroupItem(void) {
    QJsonObject tData;
    tData.insert("GroupName", GROUPITEM_NAME_DEFAULT);

    m_ptSocketClient->onSendMessage(SMT_ADDGROUP, tData);
}

void Friend::sendToRenameGroupItem(const int iIndex, const QString& rsName) {
    QJsonObject tData;
    tData.insert("GroupIndex", iIndex);
    tData.insert("GroupName", rsName);

    m_ptSocketClient->onSendMessage(SMT_RENAMEGROUP, tData);
}

void Friend::sendToDelGroupItem(const int iSrcGroupIndex, const int iDestGroupIndex) {
    QJsonObject tData;
    tData.insert("SrcGroupIndex",  iSrcGroupIndex);
    tData.insert("DestGroupIndex", iDestGroupIndex);

    m_ptSocketClient->onSendMessage(SMT_DELGROUP, tData);
}

void Friend::sendToSearchFriendItem(const int iGroupIndex, const QString& rsFriendId) {
    QJsonObject tData;
    tData.insert("GroupIndex", iGroupIndex);
    tData.insert("Id", rsFriendId);        // 好友Id

    m_ptSocketClient->onSendMessage(SMT_SEARCHFRIEND, tData);
}

void Friend::sendToAddFriendItemRequest(const int iGroupIndex, const QString& rsFriendId) {
    QJsonObject tData;
    tData.insert("GroupIndex", iGroupIndex);
    tData.insert("Id", rsFriendId);        // 好友Id

    m_ptSocketClient->onSendMessage(SMT_ADDFRIENDSENDREQUEST, tData);
}

void Friend::sendToAddFriendItemRequestReady(void) {
    m_ptSocketClient->onSendMessage(SMT_ADDFRIENDRECVREADY, 0);
}

void Friend::sendToAddFriendItem(const int iGroupIndex, const QString& rsFriendId) {
    QJsonObject tData;
    tData.insert("GroupIndex", iGroupIndex);
    tData.insert("Id", rsFriendId);        // 好友Id

    m_ptSocketClient->onSendMessage(SMT_ADDFRIEND, tData);
}

void Friend::sendToMoveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const QString& rsId, const int iIndex) {
    QJsonObject tData;
    tData.insert("SrcGroupIndex",  iSrcGroupIndex);
    tData.insert("DestGroupIndex", iDestGroupIndex);
    tData.insert("Id",             rsId);
    tData.insert("FriendIndex",    iIndex);

    m_ptSocketClient->onSendMessage(SMT_MOVEFRIEND, tData);
}

void Friend::sendToDelFriendItem(const int iGroupIndex, const QString &rsId, const int iIndex) {
    QJsonObject tData;
    tData.insert("GroupIndex",  iGroupIndex);
    tData.insert("FriendIndex", iIndex);
    tData.insert("Id",          rsId);

    m_ptSocketClient->onSendMessage(SMT_DELFRIEND, tData);
}

void Friend::sendToGetFriendList(const int iGroupIndex) {
    QJsonObject tData;
    tData.insert("GroupIndex", iGroupIndex);

    m_ptSocketClient->onSendMessage(SMT_GETFRIENDLIST, tData);
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
            addGroupItemControls(tObj.value("GroupIndex").toInt());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
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
            // 重命名分组
            renameGroupItemControls(tObj.value("GroupIndex").toInt(), tObj.value("GroupName").toString());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
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
            delGroupItemControls(tObj.value("SrcGroupIndex").toInt(), tObj.value("DestGroupIndex").toInt());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseSearchFriendItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_SEARCHFRIEND_SUCCESS == iStatus) {
            int iIndex      = tObj.value("GroupIndex").toInt();
            QJsonValue tVal = tObj.value("Friend");
            // 解析出FriendInfo
            FriendInfo tUserInfo = { 0 };
            tUserInfo.iGroup = iIndex;
            parseFriendInfo(tVal, tUserInfo);
            // 显示搜索结果
            m_ptAddFriend->showStackPage(1, tUserInfo);
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseAddFriendItemSendRequest(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_ADDFRIENDSENDREQUEST_SUCCESS == iStatus) {
            // 退出添加好友界面显示
            m_ptAddFriend->showStackPage(0, { 0 });
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseAddFriendItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_ADDFRIEND_SUCCESS == iStatus) {
            int iIndex      = tObj.value("GroupIndex").toInt();
            QJsonValue tVal = tObj.value("Friend");
            // 解析出FriendInfo
            FriendInfo tUserInfo = { 0 };
            tUserInfo.iGroup = iIndex;
            parseFriendInfo(tVal, tUserInfo);
            // 添加好友变量并设置界面
            addFriendItemControls(iIndex, tUserInfo);
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseMoveFriendItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_MOVEFRIEND_SUCCESS == iStatus) {
            moveFriendItemControls(tObj.value("SrcGroupIndex").toInt(), tObj.value("DestGroupIndex").toInt(),
                                   tObj.value("FriendIndex").toInt());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseDelFriendItem(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_DELFRIEND_SUCCESS == iStatus) {
            delFriendItemControls(tObj.value("GroupIndex").toInt(), tObj.value("FriendIndex").toInt());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseGetFriendList(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        QJsonArray  tArr;
        if (tObj.value("Friend").isArray()) {
            tArr = tObj.value("Friend").toArray();
        }
        int iStatus = tObj.value("Status").toInt();
        int iIndex  = tObj.value("GroupIndex").toInt();
        int iSize   = tArr.size();
        if (SST_GETFRIENDLIST_SUCCESS == iStatus && iSize) {
            // 填充FriendItem
            FriendInfo tUserInfo = { 0 };
            tUserInfo.iGroup = iIndex;
            for (int i = 0; i < iSize; ++i) {
                QJsonValue tVal = tArr.at(i);
                parseFriendInfo(tVal, tUserInfo);
                initFriendItemListAppend(iIndex, tUserInfo);
            }
            // 初始化界面
            initFriendItemControls(iIndex);
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseFriendInfo(const QJsonValue& rtData, FriendInfo& rtFriendInfo) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        // 解析出FriendInfo
        rtFriendInfo.bOnline = (bool)tObj.value("Online").toInt();
        rtFriendInfo.sHead   = tObj.value("Head").toString();
        if (0 == rtFriendInfo.sHead.length()) {
            rtFriendInfo.sHead = USER_HEAD_DEFAULT;
        }
        rtFriendInfo.sId     = tObj.value("Id").toString();
        rtFriendInfo.sIp     = tObj.value("Ip").toString();
        rtFriendInfo.sName   = tObj.value("Name").toString();
    }
}

void Friend::onSigMessage(int reType/* const Smt& reType */, const QJsonValue& rtData) {
    switch (reType) {
        case SMT_ADDGROUP: {
            parseAddGroupItem(rtData);
            break;
        }
        case SMT_RENAMEGROUP: {
            parseRenameGroupItem(rtData);
            break;
        }
        case SMT_DELGROUP: {
            parseDelGroupItem(rtData);
            break;
        }
        case SMT_SEARCHFRIEND: {
            parseSearchFriendItem(rtData);
            break;
        }
        case SMT_ADDFRIENDSENDREQUEST: {
            parseAddFriendItemSendRequest(rtData);
            break;
        }
        case SMT_ADDFRIEND: {
            parseAddFriendItem(rtData);
            break;
        }
        case SMT_MOVEFRIEND: {
            parseMoveFriendItem(rtData);
            break;
        }
        case SMT_DELFRIEND: {
            parseDelFriendItem(rtData);
            break;
        }
        case SMT_GETFRIENDLIST: {
            parseGetFriendList(rtData);
            break;
        }
    }
}

void Friend::onSigStatus(int reType/* const Sst& reType */) {
    switch (reType) {
        case SST_GETFRIENDLIST_FAILED: {
            QMessageBox tBox(QMessageBox::Warning, QStringLiteral("错误"),
                             QStringLiteral("初始化失败！\n请重试！"));
            tBox.setStandardButtons(QMessageBox::Ok);
            tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
            tBox.exec();
            break;
        }
        // 其他错误处理还没写
    }
}
