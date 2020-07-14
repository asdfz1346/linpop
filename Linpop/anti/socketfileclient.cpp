
#include <socketfileclient.h>
#include <common.h>
#include <clientfile.h>

#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QFileDialog>

struct SocketFileHead {
    char FileName[100];
    char TargetId[52];
    int  iFileLen;
};

SocketFileClient::SocketFileClient(QObject* ptParent) : QObject(ptParent) {
    m_ptTcpSocket = new QTcpSocket(this);

    loadSize            = 50 * 1024;
    ullSendTotalBytes   = 0;
    ullRecvTotalBytes   = 0;
    bytesWritten        = 0;
    bytesToWrite        = 0;
    bytesReceived       = 0;

    fileNameSize        = 0;
    m_bBusy = false;

    fileToSend = new QFile(this);
    fileToRecv = new QFile(this);

    connect(m_ptTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(onUpdateClientProgress(qint64)));
    connect(m_ptTcpSocket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    connect(m_ptTcpSocket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_ptTcpSocket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
}

SocketFileClient::~SocketFileClient() {
    delete m_ptTcpSocket;
    delete fileToSend;
    delete fileToRecv;
}

void SocketFileClient::startConnect(const QString& rsServerIpAddr, const int iPort) {
    if (m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->abort();
    }

    m_ptTcpSocket->connectToHost(rsServerIpAddr, iPort);
}

bool SocketFileClient::checkConnect(void) {
    if (!m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->connectToHost(g_tServerIpAddr, SOCKET_PORT_DEFAULT);
        m_ptTcpSocket->waitForConnected(1000);
    }
    if (!m_ptTcpSocket->isOpen()) {
        return false;
    }
    return true;
}

void SocketFileClient::closeConnect(void) {
    if (m_ptTcpSocket->isOpen()) {
        m_ptTcpSocket->abort();
    }
}

bool SocketFileClient::isBusy(void) {
    return m_bBusy;
}

void SocketFileClient::startTransfer(const QString& sName) {
    if (m_bBusy) {
        return;
    }

    // 如果没有连接服务器，重新连接下
    if (!m_ptTcpSocket->isOpen()) {
        startConnect(g_tServerIpAddr);
    }

    connect(m_ptTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(onUpdateClientProgress(qint64)));

    // 要发送的文件
    fileToSend = new QFile(sName);

    if (!fileToSend->open(QFile::ReadOnly)) {
        return;
    }

    // 当前文件名，不包含路径
    QString currentFileName = sName.right(sName.size() - sName.lastIndexOf('/')-1);

    SocketFileHead* ptHead = (SocketFileHead*)malloc(sizeof(SocketFileHead));
    memset(ptHead, 0, sizeof(SocketFileHead));
    memcpy(ptHead, currentFileName.toStdString().c_str(), 100);
    ptHead->iFileLen = (int)fileToSend->size();

    m_ptTcpSocket->write((char*)ptHead, sizeof(SocketFileHead));
    free(ptHead);

    // 文件总大小
    ullSendTotalBytes = fileToSend->size() + sizeof(SocketFileHead);

    // 文件数据流
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_9);

    // 发送完头数据后剩余数据的大小
    bytesToWrite = ullSendTotalBytes;

    outBlock.resize(0);
    m_bBusy = true;
}

void SocketFileClient::endTransfer(void) {
    m_bBusy = false;

    ullSendTotalBytes   = 0;
    ullRecvTotalBytes   = 0;
    bytesWritten        = 0;
    bytesToWrite        = 0;
    bytesReceived       = 0;
    fileNameSize        = 0;
}

void SocketFileClient::onSendMessage(int reType, const QJsonValue& rtData) {
    // 如果没有连接服务器，重新连接下
    if (!m_ptTcpSocket->isOpen()) {
        startConnect(g_tServerIpAddr);
    }

    disconnect(m_ptTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(onUpdateClientProgress(qint64)));
    QJsonObject tJson;
    tJson.insert("Type", reType);
    tJson.insert("Data", rtData);

    QJsonDocument tDocm;
    tDocm.setObject(tJson);
    m_ptTcpSocket->write(tDocm.toJson(QJsonDocument::Compact));
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << reType << rtData;
#endif
}

void SocketFileClient::onUpdateClientProgress(qint64 numBytes) {
    // 已经发送数据的大小
    bytesWritten += (int)numBytes;
    if (0 < bytesToWrite) {
        // 每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，就发送剩余数据的大小
        outBlock = fileToSend->read(qMin(bytesToWrite, loadSize));

        // 发送完一次数据后还剩余数据的大小
        bytesToWrite -= (int)m_ptTcpSocket->write(outBlock);

        // 清空发送缓冲区
        outBlock.resize(0);
    }
    else {
        // 如果没有发送任何数据，则关闭文件
        if (fileToSend->isOpen())
            fileToSend->close();
    }

    // 发送进度信息
    Q_EMIT sigUpdateProgress(bytesWritten, ullSendTotalBytes);

    // 发送完毕
    if (bytesWritten >= ullSendTotalBytes) {
        if (fileToSend->isOpen())
            fileToSend->close();

        bytesWritten = 0;
        ullSendTotalBytes = 0;
        bytesToWrite = 0;
        // 文件发送完成，复位相关信息
        Q_EMIT sigSendFinished();

        endTransfer();
        if (fileToSend->isOpen())
            fileToSend->close();
        m_ptTcpSocket->close();
    }
}

static SocketFileHead g_tHead = { 0 };
void SocketFileClient::onReadyRead() {
//    connect(m_ptTcpSocket, SIGNAL(bytesWritten(qint64)), this, SLOT(onUpdateClientProgress(qint64)));

    QDataStream in(m_ptTcpSocket);
    in.setVersion(QDataStream::Qt_5_9);

    // 头文件信息
    if (bytesReceived < sizeof(SocketFileHead)) {
        int nlen = sizeof(SocketFileHead);
        // 接收文件名和文件大小
        if ((m_ptTcpSocket->bytesAvailable() >= nlen) && (fileNameSize == 0)) {
            char tTemp[sizeof(SocketFileHead)] = { 0 };
            in.readRawData((char*)(&tTemp[0]), sizeof(SocketFileHead));
            memcpy((char*)&g_tHead, tTemp, 100);
            memcpy((char*)&g_tHead + 152, tTemp + 152, 4);
#ifdef _DEBUG_STATE
            qDebug() << __FUNCTION__ << __LINE__ << g_tHead.FileName << g_tHead.iFileLen;
#endif
            ullRecvTotalBytes = g_tHead.iFileLen + nlen;
            fileReadName      = QString("%1%2").arg(CLIENT_FILE_DIR).arg(g_tHead.FileName);
            fileNameSize      = strlen(g_tHead.FileName);

            if (0 != ullRecvTotalBytes) {
                bytesReceived += nlen;
            }

            fileToRecv->setFileName(fileReadName);
            if (!fileToRecv->open(QFile::WriteOnly | QIODevice::Truncate)) {
                return;
            }
        }
    }

    // 如果接收的数据小于总数据，那么写入文件
    if (bytesReceived < ullRecvTotalBytes) {
        bytesReceived += m_ptTcpSocket->bytesAvailable();
        inBlock = m_ptTcpSocket->readAll();

        if (fileToRecv->isOpen())
            fileToRecv->write(inBlock);

        inBlock.resize(0);
    }

    // 接收在底层完成，不在界面中做显示

    // 接收数据完成时
    if ((bytesReceived >= ullRecvTotalBytes) && (0 != ullRecvTotalBytes)) {
        fileToRecv->close();
        bytesReceived = 0;
        ullRecvTotalBytes = 0;
        fileNameSize = 0;
        memset((char*)&g_tHead, 0, sizeof(SocketFileHead));

#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << "Recv File Ok" << fileToRecv->fileName();
#endif
        // 数据接受完成
        endTransfer();
    }
}

void SocketFileClient::onConnected() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
//    Q_EMIT sigStatus(SFST_CONNECTED);
}

void SocketFileClient::onDisConnected() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    endTransfer();

    m_ptTcpSocket->abort();
//    Q_EMIT sigStatus(SFST_DISCONNECTED);
}
