
#include <socketclient.h>
#include <common.h>

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>

SocketClient::SocketClient(QObject* ptParent) : QObject(ptParent) {
    m_ptTcpSocket = new QTcpSocket(this);

    connect(m_ptTcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_ptTcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_ptTcpSocket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
}

SocketClient::~SocketClient() {
    delete m_ptTcpSocket;
}

void SocketClient::startConnect(const QString& rsServerIpAddr, const int iPort) {
    if (m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->abort();
    }

    m_ptTcpSocket->connectToHost(rsServerIpAddr, iPort);
}

bool SocketClient::checkConnect(void) {
    if (!m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->connectToHost(g_tServerIpAddr, SOCKET_PORT_DEFAULT);
        m_ptTcpSocket->waitForConnected(1000);
    }
    if (!m_ptTcpSocket->isOpen()) {
        return false;
    }
    return true;
}

void SocketClient::closeConnect(void) {
    if (m_ptTcpSocket->isOpen()) {
		m_ptTcpSocket->abort();
    }
}

void SocketClient::onSendMessage(int reType/* const Smt& reType */, const QJsonValue& rtData) {
    // 判断连接
    if (!checkConnect()) {
        return ;
    }

    QJsonObject tJson;
    tJson.insert("Type", reType);
    tJson.insert("Data", rtData);

    QJsonDocument tDocm;
    tDocm.setObject(tJson);
    m_ptTcpSocket->write(tDocm.toJson(QJsonDocument::Compact));
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
}

void SocketClient::onReadyRead() {
    QByteArray      tReadArray = m_ptTcpSocket->readAll();
    QJsonParseError tJsonError;
    QJsonDocument   tDocm = QJsonDocument::fromJson(tReadArray, &tJsonError);
    if (!tDocm.isNull() && (QJsonParseError::NoError == tJsonError.error)) {
        if (tDocm.isObject()) {
            QJsonObject tObj = tDocm.object();
            QJsonValue tData = tObj.value("Data");

            int iType = tObj.value("Type").toInt();
#ifdef _DEBUG_STATE
            qDebug() << __FUNCTION__ << __LINE__ << iType;
#endif
            switch (iType) {
                case SMT_LOGIN: {
                    parseLoginUserInfo(tData);
                    break;
                }
                case SMT_LOGOUT: {
                    closeConnect();
                    break;
                }
                case SMT_REGISTER: {
                    parseReisterUserInfo(tData);
                    break;
                }
                case SMT_GETGROUP: {
                    parseGroupList(tData);
                    break;
                }
                case SMT_GETFRIEND: {
                    Q_EMIT sigMessage(SMT_GETFRIEND, tData);
                    break;
                }
                case SMT_ADDFRIEND: {
                    Q_EMIT sigMessage(SMT_ADDFRIEND, tData);
                    break;
                }
                case SMT_MATCHTIPS: {
                    break;
                }
                case SMT_MODIFYPASSWORD: {
                    break;
                }

            }
        }
    }
}

void SocketClient::onConnected() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    Q_EMIT sigStatus(SST_CONNECTED);
}

void SocketClient::onDisConnected() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    m_ptTcpSocket->abort();
    Q_EMIT sigStatus(SST_DISCONNECTED);
}

void SocketClient::parseLoginUserInfo(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rtData;
#endif
    if (rtData.isObject()) {
        QJsonObject tObj = rtData.toObject();

        int iStatus = tObj.value("Status").toInt();
        if (SST_LOGIN_SUCCESS == iStatus) {
            g_tMyselfInfo.sName = tObj.value("Name").toString();
            g_tMyselfInfo.sIp   = tObj.value("Ip").toString();
            g_tMyselfInfo.sHead = tObj.value("Head").toString();
            if (0 == g_tMyselfInfo.sHead.length()) {
                // 设置默认值
                g_tMyselfInfo.sHead = USER_HEAD_DEFAULT;
            }
        }
        Q_EMIT sigStatus(iStatus);
    }
}

void SocketClient::parseReisterUserInfo(const QJsonValue& rtData) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    if (rtData.isObject()) {
        Q_EMIT sigStatus(rtData.toObject().value("Status").toInt());
    }
}

void SocketClient::parseGroupList(const QJsonValue& rtData) {
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
        if ((SST_GETGROUP_SUCCESS == iStatus) && tArr.size()) {
            g_lsGroupTextList.clear();
            for (int i = 0; i < tArr.size(); ++i) {
                g_lsGroupTextList.append(tArr.at(i).toString());
            }

            Q_EMIT sigStatus(SST_GETGROUP_SUCCESS);
            return ;
        }
        Q_EMIT sigStatus(SST_GETGROUP_FAILED);
        return ;
    }
}
