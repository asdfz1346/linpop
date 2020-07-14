
#include <chat.h>
#include <ui_chat.h>
#include <clientfile.h>

#include <QPalette>
#include <QFileDialog>
#include <QDesktopServices>
#include <QJsonObject>
#include <QCoreApplication>

static QString calcSize(qint64 iBytes);
static QString calcSpeed(double dSpeed);

static int g_iRepeat = 0;

Chat::Chat(const FriendPosition &rtPosition, const QString& rsId, const QString& rsName,
           const QString& rsIp, QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::Chat) {
    m_ptUi->setupUi(this);

    m_ptUi->winMsg->setOpenLinks(false);
    m_ptUi->winMsg->forwardAvailable(false);

    m_ptUi->recordBrowser->setOpenLinks(false);
    m_ptUi->recordBrowser->forwardAvailable(false);

    setRecordWidgetVisible(false);
    m_ptUi->fileWidget->setVisible(false);
    m_ptUi->parentWidget->setVisible(false);
    setTitleString(rsName, rsIp);

    connect(m_ptUi->winMsg, SIGNAL(anchorClicked(QUrl)), this, SLOT(onMsgAnchorClicked(QUrl)));
    connect(m_ptUi->recordBrowser, SIGNAL(anchorClicked(QUrl)), this, SLOT(onRcdAnchorClicked(QUrl)));

    m_ptFriend = Friend::getInstance();
    m_sId      = rsId;
    m_sName    = rsName;
    m_tPosition.iFriendIndex = rtPosition.iFriendIndex;
    m_tPosition.iGroupIndex  = rtPosition.iGroupIndex;

    m_bIsParentEven = false;
    m_bIsRecordEven = false;
    m_bIsTransform  = false;

    // 文件服务器
    m_tcpFileSocket = new SocketFileClient(this);

    connect(m_tcpFileSocket, SIGNAL(sigFileRecvOk(QString)), this, SLOT(SltFileRecvFinished(QString)));
    connect(m_tcpFileSocket, SIGNAL(sigUpdateProgress(quint64,quint64)), this, SLOT(SltUpdateProgress(quint64,quint64)));
}

Chat::~Chat() {
    delete m_ptUi;
}

void Chat::setTitleString(const QString& rsName, const QString& rsIp) {
    QString sTemp;
    if (rsIp.length()) {
        sTemp = rsName + " <" + rsIp + ">";
    }
    else {
        sTemp = rsName + QStringLiteral(" <离线>");
    }
    this->setWindowTitle(sTemp);

    m_sName = rsName;
}

void Chat::addSendMessageItemControls(const int iMessageType, const QString& rsString, const QString& rsTime) {
    Q_UNUSED(iMessageType);

    m_ptUi->winMsg->append("<font color=\"#FF0000\">" + g_tMyselfInfo.sName + "</font>  " + rsTime);
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << rsString;
#endif
    m_ptUi->winMsg->append("<font color=\"#000000\">" + rsString + "</font><p></p>");
    m_ptUi->msgInput->clear();
}

void Chat::addRecvMessageItemControls(const int iMessageType, const QString& rsString, const QString& rsTime) {
    Q_UNUSED(iMessageType);

    m_ptUi->winMsg->append("<font color=\"#FFAA00\">" + m_sName + "</font>  " + rsTime);
    m_ptUi->winMsg->append("<font color=\"#000000\">" + rsString + "</font><p></p>");
    m_ptUi->msgInput->setFocus();
}

void Chat::addHistoryItemControls(const int iMessageType, const QString& rsString, const QString& rsTime,
                                  const QString& rsId) {
    Q_UNUSED(iMessageType);

    if (rsId == g_tMyselfInfo.sId) {
        m_ptUi->recordBrowser->append("<font color=\"#FF0000\">" + g_tMyselfInfo.sName + "</font>  " + rsTime);
    }
    else {
        m_ptUi->recordBrowser->append("<font color=\"#FFAA00\">" + m_sName + "</font>  " + rsTime);
    }
    m_ptUi->recordBrowser->append("<font color=\"#000000\">" + rsString + "</font><p></p>");
    m_ptUi->recordBrowser->setFocus();
}

void Chat::on_sendButton_clicked() {
    QString sTemp = m_ptUi->msgInput->toPlainText();
    if (sTemp.length()) {
        m_ptFriend->sendToSendMessage(SCMT_STRING, sTemp, m_tPosition.iGroupIndex, m_sId,
                                      m_tPosition.iFriendIndex);
    }
}

void Chat::on_toolButton_clicked() {
    if (!m_bIsParentEven) {
        m_bIsParentEven = true;
        m_ptUi->parentWidget->setVisible(true);
    }
    else if (m_bIsParentEven && (!m_bIsTransform)) {
        m_bIsParentEven = false;
        m_ptUi->parentWidget->setVisible(false);
    }
    m_bIsRecordEven = !m_bIsRecordEven;
    setRecordWidgetVisible(m_bIsRecordEven);

    if (m_bIsRecordEven) {
        // 获取历史纪录
        m_ptUi->recordBrowser->clear();
        m_ptFriend->sendToGetHistory(m_tPosition.iGroupIndex, m_sId, m_tPosition.iFriendIndex);
    }
}

void Chat::on_searchEdit_editingFinished() {
    if (m_ptUi->searchEdit->hasFocus()) {
        this->setFocus();
    }
    else {
        m_ptUi->recordBrowser->moveCursor(QTextCursor::Start);
    }
}

void Chat::on_searchButton_clicked() {
    QString sFindText = m_ptUi->searchEdit->text();
    if (0 == sFindText.length()) {
        return ;
    }
    if (m_ptUi->recordBrowser->find(sFindText)) {
        QPalette tPalette = m_ptUi->recordBrowser->palette();
        tPalette.setColor(QPalette::Highlight, tPalette.color(QPalette::Active, QPalette::Highlight));
        m_ptUi->recordBrowser->setPalette(tPalette);
    }
}

void Chat::on_fileButton_clicked() {
    if (m_tcpFileSocket->isBusy()) {
        return ;
    }

    QString sPath = QFileDialog::getOpenFileName(this, QStringLiteral("选择文件"), CLIENT_FILE_DIR, "All File(*)");
    if (sPath.isEmpty()) {
        return;
    }

    QFileInfo tFileInfo(sPath);

    // 文件上传限制
    if (tFileInfo.size() > (1024 * 1024 * 1024)) {
        return;
    }

    // 为了保证消息一致性，需要将文件复制到File/下
    QString sFileName = sPath.right(sPath.size() - sPath.lastIndexOf('/') - 1);
    m_strFileName = CLIENT_FILE_DIR + sFileName;

    QFile tFile(m_strFileName);
    if (!tFile.exists()) {
        if (QFile::copy(sPath, m_strFileName)) {
            /* Nothing */
        }
        else {
            return ;
        }
    }

    if (!m_bIsParentEven) {
        m_bIsParentEven = true;
        m_ptUi->parentWidget->setVisible(true);
    }

    m_bIsTransform = true;
    m_ptUi->fileWidget->setVisible(true);
    // 关闭界面放在完成传输

    // 标志位
    g_iRepeat = 0xA0;

    // 开始传输文件
    m_tcpFileSocket->startTransfer(m_strFileName);

    // 开始计时
    m_updateTime.restart();
}

void Chat::imgPathToHtml(QString& rsPath) {
     rsPath = QString("<img src=\"%1\"/>").arg(rsPath);
}

void Chat::setRecordWidgetVisible(bool bIsVisible) {
    m_ptUi->searchEdit->setVisible(bIsVisible);
    m_ptUi->recordBrowser->setVisible(bIsVisible);
    m_ptUi->searchButton->setVisible(bIsVisible);
}

void Chat::sendToRecvFile(const QString& rsFileName) {
    QJsonObject tData;
    tData.insert("FileName", rsFileName);

    m_tcpFileSocket->onSendMessage(SFT_RECVFILE, tData);
}

void Chat::SltFileRecvFinished(const QString &filePath) {
    if (filePath.isEmpty()) {
        return;
    }

//    m_ptUi->winMsg->append(QString("%1 %2\n").arg(QStringLiteral("文件接收完成：")).arg(filePath));
}

void Chat::SltUpdateProgress(quint64 bytes, quint64 total) {
    // 总时间
    int nTime = m_updateTime.elapsed();

    // 下载速度
    double dBytesSpeed = (bytes * 1000.0) / nTime;

    m_ptUi->lineEditCurrSize->setText(calcSize(bytes));
    m_ptUi->lineEditTotalSize->setText(calcSize(total));
    m_ptUi->lineEditRate->setText(calcSpeed(dBytesSpeed));

    m_ptUi->progressBar->setMaximum(total);
    m_ptUi->progressBar->setValue(bytes);

   // 文件接收完成，发送消息给服务器，转发至对端
    if ((0xA0 == g_iRepeat) && (bytes >= total)) {
        // 发送消息
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << "sendToSendMessage SCMT_STRING";
#endif
        m_ptFriend->sendToSendMessage(SCMT_STRING, QString("<a href=\"%1\">%2</a>").arg(m_strFileName).arg(m_strFileName),
                                     m_tPosition.iGroupIndex, m_sId, m_tPosition.iFriendIndex);
        // 关闭传输界面
        m_bIsTransform = false;
        m_ptUi->fileWidget->setVisible(false);
        if (!m_bIsRecordEven) {
           m_bIsParentEven = false;
           m_ptUi->parentWidget->setVisible(false);
        }
        g_iRepeat = 0;
    }
}

void Chat::onMsgAnchorClicked(const QUrl& rtUrl) {
    QString sFilePath = rtUrl.toString();
    QFile   tFile(sFilePath);
    if (!tFile.exists()) {
        // 接收文件
        QString sFileName = QString(sFilePath.right(sFilePath.size() - sFilePath.lastIndexOf('/') - 1));
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << sFileName;
#endif
        sendToRecvFile(sFileName);
    }
    else {
        QString sPath = QString("%1/%2").arg(QDir::currentPath()).arg(CLIENT_FILE_DIR);
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << sPath;
#endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
    }
}

void Chat::onRcdAnchorClicked(const QUrl& rtUrl) {
    QString sFilePath = rtUrl.toString();
    QFile   tFile(sFilePath);
    if (!tFile.exists()) {
        // 接收文件
        QString sFileName = QString(sFilePath.right(sFilePath.size() - sFilePath.lastIndexOf('/') - 1));
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << sFileName;
#endif
        sendToRecvFile(sFileName);
    }
    else {
        QString sPath = QString("%1/%2").arg(QDir::currentPath()).arg(CLIENT_FILE_DIR);
#ifdef _DEBUG_STATE
        qDebug() << __FUNCTION__ << __LINE__ << sPath;
#endif
        QDesktopServices::openUrl(QUrl::fromLocalFile(sPath));
    }
}

static QString calcSize(qint64 iBytes) {
    QString sUnit;
    double dSize = iBytes * 1.0;
    if (dSize <= 0) {
        dSize = 0.0;
    }
    else if (dSize < 1024) {
        sUnit = "Bytes";
    }
    else if (dSize < 1024 * 1024) {
        dSize /= 1024;
        sUnit = "KB";
    }
    else if (dSize < 1024 * 1024 * 1024) {
        dSize /= (1024 * 1024);
        sUnit = "MB";
    }
    else {
        dSize /= (1024 * 1024 * 1024);
        sUnit = "GB";
    }

    return QString("%1 %2").arg(QString::number(dSize, 'f', 2)).arg(sUnit);
}

static QString calcSpeed(double dSpeed) {
    QString sUnit;
    if (dSpeed <= 0) {
        dSpeed = 0;
        sUnit = "Bytes/S";
    }
    else if (dSpeed < 1024) {
        sUnit = "Bytes/S";
    }
    else if (dSpeed < 1024 * 1024) {
        dSpeed /= 1024;
        sUnit = "KB/S";
    }
    else if (dSpeed < 1024 * 1024 * 1024) {
        dSpeed /= (1024 * 1024);
        sUnit = "MB/S";
    }
    else {
        dSpeed /= (1024 * 1024 * 1024);
        sUnit = "GB/S";
    }

    QString sSpeed = QString::number(dSpeed, 'f', 2);
    return QString("%1 %2").arg(sSpeed).arg(sUnit);
}
