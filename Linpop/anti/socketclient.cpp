
#include <socketclient.h>
#include <common.h>

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

SocketClient::SocketClient(QObject* ptParent) : QObject(ptParent) {
    m_ptTcpSocket = new QTcpSocket(this);

    connect(m_ptTcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_ptTcpSocket, SIGNAL(connected()), this, SLOT(onConnect()));
    connect(m_ptTcpSocket, SIGNAL(disconnected()), this, SLOT(onDisconnect()));
}

SocketClient::~SocketClient() {
    onSendOfflineMessage();
}

void SocketClient::checkConnect(void) {
    if (QTcpSocket::ConnectedState != m_ptTcpSocket->state()) {
        m_ptTcpSocket->connectToHost(g_tServerIpAddr, SOCKET_PORT_DEFAULT);
    }
}

void SocketClient::closeConnect(void) {
    if (m_ptTcpSocket->isOpen()) {
		m_ptTcpSocket->abort();
    }
}

void SocketClient::connectToServer(const QString& rsServerIpAddr, const int iPort) {
    if (m_ptTcpSocket->isOpen()) {
		m_ptTcpSocket->abort();
    }
	
    m_ptTcpSocket->connectToHost(rsServerIpAddr, iPort);
}

void SocketClient::onSendMessage(const Smt& reType, const QJsonValue& rtData) {
    // 连接服务器
    if (!m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->connectToHost(g_tServerIpAddr, SOCKET_PORT_DEFAULT);
        m_ptTcpSocket->waitForConnected(1000);
    }
    if (!m_ptTcpSocket->isOpen()) {
        return ;
    }

    QJsonObject tJson;
    tJson.insert("Type", reType);
    tJson.insert("Data", rtData);

    QJsonDocument tDocm;
    tDocm.setObject(tJson);
    m_ptTcpSocket->write(tDocm.toJson(QJsonDocument::Compact));
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__;
#endif
}

void SocketClient::onSendOnlineMessage() {

}

void SocketClient::onSendOfflineMessage() {

}

void SocketClient::onReadyRead() {

}

void SocketClient::onDisconnect() {
    m_ptTcpSocket->abort();
}

void SocketClient::onConnect() {

}
