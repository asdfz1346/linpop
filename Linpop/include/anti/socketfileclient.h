
#ifndef SOCKETFILECLIENT_H
#define SOCKETFILECLIENT_H

#include <config.h>

#include <QObject>
#include <QString>
#include <QTcpSocket>
#include <QJsonValue>
#include <QFile>

typedef enum SOCKET_FILE_TYPE {
    SFT_UNKNOW,

    SFT_SENDFILE                = 0x90,     // 发送文件
    SFT_RECVFILE,                           // 接收文件
} Slt;

typedef enum SOCKET_FILE_STATUS_TYPE {
    SFST_UNKNOW,

    SFST_CONNECTED              = 0x90,     // 用于发送连接信号
    SFST_DISCONNECTED,                      // 用于发送取消连接信号

    SFST_SENDFILE_SUCCESS       = 0x95,     // 发送成功
    SFST_SENDFILE_FAILED,
    SFST_RECVFILE_SUCCESS,                  // 接收成功
    SFST_RECVFILE_FAILED,
} Sfst;

class SocketFileClient : public QObject {
    Q_OBJECT

public:
    explicit SocketFileClient(QObject* ptParent = nullptr);
    ~SocketFileClient();
    // 连接服务器操作
    void startConnect(const QString& rsServerIpAddr = SOCKET_SERVER_DEFAULT,
                      const int iPort = SOCKET_PORT_DEFAULT);
    bool checkConnect(void);
    void closeConnect(void);
    bool isBusy(void);

    // 开始传输
    void startTransfer(const QString& sName);
    // 结束传输
    void endTransfer(void);

signals:
    void sigSendFinished();
    void sigFileRecvOk();
    void sigUpdateProgress(quint64 currSize, quint64 total);
    void sigConnected();

public slots:
    // 发送消息函数
    void onSendMessage(int reType, const QJsonValue& rtData);

private slots:
    // 更新进度条
    void onUpdateClientProgress(qint64);
    // 接收消息函数
    void onReadyRead();
    // 连接服务器的槽函数
    void onConnected();
    // 断开服务器的槽函数
    void onDisConnected();

private:
    QTcpSocket* m_ptTcpSocket;
    quint64     loadSize;           // 每次发送数据的大小

    // 发送
    quint16     blockSize;          // 存放接收到的信息大小
    quint64     ullSendTotalBytes;  // 数据总大小
    quint64     bytesWritten;       // 已经发送数据大小
    quint64     bytesToWrite;       // 剩余数据大小
    QByteArray  outBlock;           // 数据缓冲区，即存放每次要发送的数据
    QFile*      fileToSend;         // 要发送的文件

    // 接收
    quint64     ullRecvTotalBytes;  // 数据总大小
    quint64     bytesReceived;      // 已收到数据的大小
    quint64     fileNameSize;       // 文件名的大小信息
    QString     fileReadName;       // 存放文件名
    QByteArray  inBlock;            // 数据缓冲区
    QFile*      fileToRecv;         // 要发送的文件

    bool        m_bBusy;
};

#endif // SOCKETFILECLIENT_H
