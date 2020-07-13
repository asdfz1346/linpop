
#ifndef LINPOP_CHAT_H
#define LINPOP_CHAT_H

#include <friend.h>
#include <common.h>
#include <socketfileclient.h>

#include <QWidget>
#include <QString>
#include <QTime>

namespace Ui {
class Chat;
}

class Friend;

class Chat : public QWidget {
    Q_OBJECT

public:
    explicit Chat(const FriendPosition &rtPosition, const QString& rsId, const QString& rsName,
                  const QString& rsIp, QWidget* ptParent = nullptr);
    ~Chat();
    void setTitleString(const QString& rsName, const QString& rsIp);

    // 传输相关
    void addSendMessageItemControls(const int iMessageType, const QString &rsString, const QString& rsTime);
    void addRecvMessageItemControls(const int iMessageType, const QString& rsString, const QString &rsTime);
    void addHistoryItemControls(const int iMessageType, const QString& rsString, const QString& rsTime,
                                const QString& rsId);

private slots:
    void on_sendButton_clicked();
    void on_toolButton_clicked();
    void on_searchEdit_editingFinished();
    void on_searchButton_clicked();
    void on_fileButton_clicked();
    // 文件
    void SltFileRecvFinished(const quint8 &type, const QString &filePath);
    void SltUpdateProgress(quint64 bytes, quint64 total);

private:
    void imgPathToHtml(QString& rsPath);
    void setRecordWidgetVisible(bool bIsVisible);

    Ui::Chat* m_ptUi;
    Friend*   m_ptFriend;

    // 当前位置
    FriendPosition m_tPosition;
    // 对方Id和名字
    QString        m_sId;
    QString        m_sName;

    // 文件传输
    SocketFileClient* m_tcpFileSocket;
    QString           m_strFileName;
    QTime             m_updateTime;

    bool m_bIsParentEven;
    bool m_bIsRecordEven;
    bool m_bIsTransform;
};

#endif // LINPOP_CHAT_H
