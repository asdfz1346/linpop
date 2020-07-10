
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
                // Loggin
                case SMT_REGISTER:
                case SMT_MATCHTIPS:
                case SMT_MODIFYPASSWORD:
                case SMT_LOGIN:
                case SMT_GETGROUP: {
                    Q_EMIT sigMessage(iType, tData);
                    break;
                }
                case SMT_LOGOUT: {
                    closeConnect();
                    break;
                }
                // Friend
                case SMT_ADDGROUP:
                case SMT_RENAMEGROUP:
                case SMT_DELGROUP:
                case SMT_SEARCHFRIEND:
                case SMT_ADDFRIENDSENDREQUEST:
                case SMT_ADDFRIEND:
                case SMT_MOVEFRIEND:
                case SMT_DELFRIEND:
                case SMT_ADDFRIEND:
                case SMT_GETFRIENDLIST: {
                    Q_EMIT sigMessage(SMT_GETFRIENDLIST, tData);
                    break;
                }
                case SMT_ADDFRIEND: {
                    Q_EMIT sigMessage(SMT_ADDFRIEND, tData);
                    break;
                }
                case SMT_ADDGROUP: {
                    Q_EMIT sigMessage(SMT_ADDGROUP, tData);
                    break;
                }
                case SMT_DELGROUP: {
                    Q_EMIT sigMessage(SMT_DELGROUP, tData);
                    break;
                }
                case SMT_RENAMEGROUP: {
                    Q_EMIT sigMessage(SMT_RENAMEGROUP, tData);
                    break;
                }
                default:
                    break;
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
