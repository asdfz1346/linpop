
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
    if (m_bBusy) return;

    // 如果没有连接服务器，重新连接下
    if (!m_ptTcpSocket->isOpen()) {
        startConnect(g_tServerIpAddr);
    }

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
    memcpy(ptHead + 100, "5", 52);
    ptHead->iFileLen = (int)fileToSend->size();

    m_ptTcpSocket->write((char*)ptHead, sizeof(SocketFileHead));
    free(ptHead);

    // 文件总大小
    ullSendTotalBytes = fileToSend->size() + sizeof(SocketFileHead);

    // 文件数据流
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_9);

//    qDebug() << __FUNCTION__ << currentFileName;

    // 依次写入总大小信息空间，文件名大小信息空间，文件名
//    sendOut << qint64(0) << qint64(0) << currentFileName;

    // 这里的总大小是文件名大小等信息和实际文件大小的总和
//    ullSendTotalBytes += outBlock.size();

    // 返回outBolock的开始，用实际的大小信息代替两个qint64(0)空间
    //sendOut.device()->seek(0);
//    sendOut << ullSendTotalBytes << qint64((outBlock.size() - sizeof(qint64)*2));

    // 发送完头数据后剩余数据的大小
//    bytesToWrite = ullSendTotalBytes - m_ptTcpSocket->write(outBlock);
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

void SocketFileClient::onReadyRead() {
    QDataStream in(m_ptTcpSocket);
    in.setVersion(QDataStream::Qt_5_9);

    // 如果接收到的数据小于等于20个字节，那么是刚开始接收数据，我们保存为头文件信息
    if (bytesReceived <= (sizeof(qint64)*2)) {
        int nlen = sizeof(qint64) * 2;
        // 接收数据总大小信息和文件名大小信息
        if ((m_ptTcpSocket->bytesAvailable() >= nlen) && (fileNameSize == 0)) {
            in >> ullRecvTotalBytes >> fileNameSize;
            if (0 != ullRecvTotalBytes) {
                bytesReceived += nlen;
            }
        }

        // 接收文件名，并建立文件
        if((m_ptTcpSocket->bytesAvailable() >= (qint64)fileNameSize) &&
                ((qint64)fileNameSize != 0) && (0 != ullRecvTotalBytes)) {
            in >> fileReadName;
            fileReadName  =  CLIENT_FILE_DIR + fileReadName;
            bytesReceived += fileNameSize;

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

        qDebug() << "File recv ok" << fileToRecv->fileName();
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
