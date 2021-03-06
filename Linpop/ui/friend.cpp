﻿
#include <friend.h>
#include <ui_friend.h>
#include <clientfile.h>

#include <QFileDialog>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QFile>

Friend::Friend(QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::Friend) {
    m_ptUi->setupUi(this);
    m_ptGroup = new Group(this);
    m_ptUi->friendWidget->layout()->addWidget(m_ptGroup);
    m_ptAddFriend = nullptr;
    m_mpChatMap.clear();
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
    // 设置个人文件夹
    ClientFile::checkClientDir();
    // 发送获取好友请求
    sendToGetFriendList();
}

void Friend::postInit(/*const UserInfo& rtMyselfInfo*/) {
    // 初始化定时器，用于让服务器发送离线消息和文件
    QTimer::singleShot(3000, this, SLOT(onTimeout()));
}

GroupItem* Friend::getGroupitemIndex(const int iIndex) {
    return m_ptGroup->getGroupitemIndex(iIndex);
}

const int Friend::getDefaultGroupitemIndex(void) {
    return m_ptGroup->getDefaultGroupitemIndex();
}

const int Friend::getFriendIndexById(const int iGroupIndex, const QString& rsId) {
    return m_ptGroup->getFriendIndexById(iGroupIndex, rsId);
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

void Friend::showTipWindow(const QString& rsTitle, const QString& rsTip, const QString& rsButtonText) {
    QMessageBox tBox(QMessageBox::Warning, rsTitle, rsTip);
    tBox.setStandardButtons(QMessageBox::Ok);
    tBox.setButtonText(QMessageBox::Ok, rsButtonText);
    tBox.exec();
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

void Friend::showChatWindow(const int iGroupIndex, const int iIndex) {
    FriendPosition tPosition(iGroupIndex, iIndex);
    Chat* ptChatWindow = m_mpChatMap.key(tPosition);
    FriendInfo tTempInfo = getGroupitemIndex(iGroupIndex)->getFriendInfo(iIndex);

    if (nullptr == ptChatWindow) {
        ptChatWindow = new Chat(tPosition, tTempInfo.sId, tTempInfo.sName, tTempInfo.sIp);
        m_mpChatMap.insert(ptChatWindow, tPosition);
    }

    ptChatWindow->setTitleString(tTempInfo.sName, tTempInfo.sIp);
    ptChatWindow->show();
    ptChatWindow->setFocus();
}

void Friend::addSendMessageItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                        const QString& rsString, const QString& rsTime) {
    m_mpChatMap.key(FriendPosition(iGroupIndex, iIndex))->addSendMessageItemControls(iMessageType, rsString, rsTime);
}

void Friend::addRecvMessageItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                        const QString& rsString, const QString& rsTime) {
    m_mpChatMap.key(FriendPosition(iGroupIndex, iIndex))
            ->addRecvMessageItemControls(iMessageType, rsString, rsTime);
}

void Friend::addHistoryItemControls(const int iGroupIndex, const int iIndex, const int iMessageType,
                                        const QString& rsString, const QString& rsTime, const QString& rsId) {
    m_mpChatMap.key(FriendPosition(iGroupIndex, iIndex))
            ->addHistoryItemControls(iMessageType, rsString, rsTime, rsId);
}

void Friend::closeEvent(QCloseEvent* ptEvent) {
    Q_UNUSED(ptEvent);
    this->close();
    qApp->quit();
}

void Friend::on_headButton_clicked() {
    QString sPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择头像"), ".", "Image Files(*.png)");
    if (sPath.length()) {
   //     file.size();

        QImage tImage;
        if (tImage.load(sPath)) {
            // 更新全局用户成员
            g_tMyselfInfo.sHead = sPath;
            m_ptUi->headButton->setStyleSheet(QString("border-image: url(").append(g_tMyselfInfo.sHead + ")"));
        }
    }
}

void Friend::on_nameEdit_editingFinished() {
    if (m_ptUi->nameEdit->hasFocus()) {
        this->setFocus();
    }
    else {
        sendToUpdateName();
    }
}

void Friend::initUserInofControls(/*const UserInfo& rtMyselfInfo*/) {
    m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
    m_ptUi->idLabel->setText(g_tMyselfInfo.sId);
    m_ptUi->headButton->setStyleSheet(QString("border-image: url(").append(g_tMyselfInfo.sHead + ")"));
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

void Friend::updateFriendItemControls(const FriendInfo& rtFriendInfo, const int iFriendIndex) {
    m_ptGroup->updateFriendItemControls(rtFriendInfo, iFriendIndex);
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

void Friend::sendToUpdateFriendStatus(const int iGroupIndex, const QString& rsId, const int iIndex) {
    QJsonObject tData;
    tData.insert("GroupIndex",  iGroupIndex);
    tData.insert("FriendIndex", iIndex);
    tData.insert("Id",          rsId);

    m_ptSocketClient->onSendMessage(SMT_UPDATEFRIENDSTATUS, tData);
}

void Friend::sendToSendMessage(const int iMessageType, const QString& rsString, const int iGroupIndex,
                               const QString& rsId, const int iIndex) {
    QJsonObject tData;
    tData.insert("MessageType", iMessageType);
    tData.insert("String",      rsString);
    tData.insert("GroupIndex",  iGroupIndex);
    tData.insert("FriendIndex", iIndex);
    tData.insert("Id",          rsId);

    m_ptSocketClient->onSendMessage(SMT_SENDMESSAGE, tData);
}

void Friend::sendToGetHistory(const int iGroupIndex, const QString& rsId, const int iIndex) {
    QJsonObject tData;
    tData.insert("GroupIndex",  iGroupIndex);
    tData.insert("FriendIndex", iIndex);
    tData.insert("Id",          rsId);

    m_ptSocketClient->onSendMessage(SMT_GETHISTORY, tData);
}

void Friend::sendToGetFriendList(void) {
    m_ptSocketClient->onSendMessage(SMT_GETFRIENDLIST, 0);
}

void Friend::sendToUpdateName(void) {
    QJsonObject tData;
    tData.insert("Name", m_ptUi->nameEdit->text());

    m_ptSocketClient->onSendMessage(SMT_UPDATENAME, tData);
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
            int iGroupIndex = tObj.value("GroupIndex").toInt();
            int iFriendIndex = 0;
            if (tObj.value("IsFriendIndex").toInt()) {
                iFriendIndex = tObj.value("FriendIndex").toInt();
            }
            else {
                // 通过GroupIndex和FriendId获取FriendIndex
                iFriendIndex = getFriendIndexById(iGroupIndex, tObj.value("FriendId").toString());
            }
            delFriendItemControls(iGroupIndex, iFriendIndex);
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
        if (tObj.value("Group").isArray()) {
            tArr = tObj.value("Group").toArray();
        }
        int iStatus = tObj.value("Status").toInt();
        int iSize   = tArr.size();
        if (SST_GETFRIENDLIST_SUCCESS == iStatus && iSize) {
            // 读取所有分组的好友信息
            for (int i = 0; i < iSize; ++i) {
                QJsonValue tVal = tArr.at(i);
                if (tVal.isNull()) {
                    continue;
                }
                parseGroupFriendInfo(tVal);
            }
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseUpdateFriendStatus(const QJsonValue &rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_UPDATEFRIENDSTATUS_SUCCESS == iStatus) {
            FriendInfo tFriendInfo = { 0 };
            tFriendInfo.bOnline = (bool)tObj.value("Online").toInt();
            tFriendInfo.iGroup  = tObj.value("GroupIndex").toInt();
//            tFriendInfo.sHead   = tObj.value("Head").toString();
            tFriendInfo.sIp     = tObj.value("Ip").toString();
            tFriendInfo.sName   = tObj.value("Name").toString();
            int iFriendIndex    = tObj.value("FriendIndex").toInt();
            updateFriendItemControls(tFriendInfo, iFriendIndex);

            // 显示聊天界面
            showChatWindow(tFriendInfo.iGroup, iFriendIndex);
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseUpdateName(const QJsonValue &rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_UPDATENAME_SUCCESS == iStatus) {
            g_tMyselfInfo.sName = m_ptUi->nameEdit->text();
        }
        else {
            m_ptUi->nameEdit->setText(g_tMyselfInfo.sName);
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseSendMessage(const QJsonValue &rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iStatus = tObj.value("Status").toInt();
        if (SST_SENDMESSAGE_SUCCESS == iStatus) {
            QJsonObject tMessage = tObj.value("Message").toObject();
            addSendMessageItemControls(tObj.value("GroupIndex").toInt(), tObj.value("FriendIndex").toInt(),
                                       tObj.value("MessageType").toInt(), tMessage.value("String").toString(),
                                       tObj.value("DateTime").toString());
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseRecvMessage(const QJsonValue &rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        QJsonArray  tArr;
        if (tObj.value("Message").isArray()) {
            tArr = tObj.value("Message").toArray();
        }
        int iStatus = tObj.value("Status").toInt();
        int iSize   = tArr.size();
        if (SST_RECVMESSAGE_SUCCESS == iStatus && iSize) {
            for (int i = 0; i < iSize; ++i) {
                QJsonValue tVal = tArr.at(i);
                if (tVal.isNull()) {
                    continue;
                }
                parseGroupRecvMessage(tVal);
            }
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseGetHistory(const QJsonValue &rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        QJsonArray  tArr;
        if (tObj.value("Content").isArray()) {
            tArr = tObj.value("Content").toArray();
        }
        int iStatus = tObj.value("Status").toInt();
        int iSize   = tArr.size();
        if (SST_GETHISTORY_SUCCESS == iStatus && iSize) {
            for (int i = 0; i < iSize; ++i) {
                QJsonValue tVal = tArr.at(i);
                if (tVal.isNull()) {
                    continue;
                }
                parseGroupHistory(tVal, tObj.value("GroupIndex").toInt(), tObj.value("FriendIndex").toInt());
            }
        }
        Q_EMIT m_ptSocketClient->sigStatus(iStatus);
    }
}

void Friend::parseGroupFriendInfo(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        QJsonArray  tArr;
        if (tObj.value("Friend").isArray()) {
            tArr = tObj.value("Friend").toArray();
        }
        int iIndex = tObj.value("GroupIndex").toInt();
        int iSize  = tArr.size();
        if (iSize) {
            // 填充FriendItem
            FriendInfo tUserInfo = { 0 };
            tUserInfo.iGroup = iIndex;
            for (int i = 0; i < iSize; ++i) {
                QJsonValue tVal = tArr.at(i);
                if (tVal.isNull()) {
                    continue;
                }
                parseFriendInfo(tVal, tUserInfo);
                initFriendItemListAppend(iIndex, tUserInfo);
            }
            // 初始化界面
            initFriendItemControls(iIndex);
        }
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

void Friend::parseGroupRecvMessage(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        int iGroupIndex  = tObj.value("GroupIndex").toInt();
        QString sId      = tObj.value("Id").toString();
        // 根据Id查找好友的FriendIndex
        int iFriendIndex = getFriendIndexById(iGroupIndex, sId);
        // 显示聊天界面
        showChatWindow(iGroupIndex, iFriendIndex);
        addRecvMessageItemControls(iGroupIndex, iFriendIndex, tObj.value("MessageType").toInt(),
                                   tObj.value("String").toString(), tObj.value("DateTime").toString());
    }
}

void Friend::parseGroupHistory(const QJsonValue& rtData, const int iGroupIndex, const int iFriendIndex) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();
        addHistoryItemControls(iGroupIndex, iFriendIndex, tObj.value("MessageType").toInt(),
                               tObj.value("Message").toString(), tObj.value("DateTime").toString(),
                               tObj.value("SourceId").toString());
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
            // 取消获取返回状态，防止TCP粘包
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
        case SMT_UPDATEFRIENDSTATUS: {
            parseUpdateFriendStatus(rtData);
            break;
        }
        case SMT_UPDATENAME: {
            parseUpdateName(rtData);
            break;
        }
        case SMT_SENDMESSAGE: {
            parseSendMessage(rtData);
            break;
        }
        case SMT_RECVMESSAGE: {
            parseRecvMessage(rtData);
            break;
        }
        case SMT_GETHISTORY: {
            parseGetHistory(rtData);
            break;
        }
    }
}

void Friend::onSigStatus(int reType/* const Sst& reType */) {
    switch (reType) {
#if 0   // 非必要错误处理
        case SST_ADDGROUP_FAILED:
        case SST_RENAMEGROUP_FAILED:
        case SST_DELGROUP_FAILED:
        case SST_SEARCHFRIEND_FAILED:
        case SST_ADDFRIENDSENDREQUEST_FAILED:
        case SST_ADDFRIEND_FAILED:
        case SST_MOVEFRIEND_FAILED:
        case SST_DELFRIEND_FAILED:
        case SST_UPDATENAME_FAILED:
        case SST_UPDATEHEAD_FAILED:
        case SST_UPDATEFRIENDSTATUS_FAILED:
        case SST_SENDMESSAGE_FAILED:
        case SST_RECVMESSAGE_FAILED:
        case SST_GETHISTORY_FAILED:
#endif
        case SST_GETFRIENDLIST_FAILED: {
            showTipWindow(QStringLiteral("内部错误"), QStringLiteral("初始化失败！\n请重试！"));
            break;
        }
    }
}

void Friend::onTimeout(void) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    // 发送RECVREADY
    sendToAddFriendItemRequestReady();
}
