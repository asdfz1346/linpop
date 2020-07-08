
#include <loggin.h>
#include <ui_loggin.h>
#include <common.h>
#include <clientfile.h>

#include <iostream>
#include <fstream>

#include <QMessageBox>
#include <QTcpSocket>
#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>

Loggin::Loggin(QWidget *ptParent) : MoveableFramelessWindow(ptParent), m_ptUi(new Ui::Loggin) {
    m_ptUi->setupUi(this);
    m_ptFriend = Friend::getInstance();
    m_ptRegisterId = nullptr;
    m_ptRxv = nullptr;

    // 读取配置文件
    ClientFile::createConfigFile();
    ClientFile::readConfigFile();

    // 更新必要的变量和控件
    setServerIpInputFormat();
    setControlsByConfig();

    // 准备改成单例模式
    setSocketClient();
}

Loggin::~Loggin() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    if (m_ptRxv)
        delete m_ptRxv;

    // 回写配置文件
    ClientFile::writeConfigFile(m_ptUi->saveBox->isChecked());
    // TODO：发送退出信息
    if (m_ptRecoverPassword)
        delete m_ptRecoverPassword;
    if (m_ptRegisterId)
        delete m_ptRegisterId;
    delete m_ptUi;
}

void Loggin::sendRegisterInfo(const QString& rsId,  const QString& rsPassword,
                              const QString& rsTip, const QString& rsName) {
    QJsonObject tData;
    tData.insert("Id", rsId);
    tData.insert("Password", rsPassword);
    tData.insert("Tip", rsTip);
    tData.insert("Name", rsName);

    m_ptSocketClient->onSendMessage(SMT_REGISTER, tData);
}

QWidget* Loggin::getDragnWidget() {
    return m_ptUi->nameLabel;
}

void Loggin::on_setButton_clicked() {
    // 获取并更新全局的服务器ip
    g_tServerIpAddr = m_ptUi->serverIpEdit->text();
    // 隐藏set按键
    m_ptUi->stackedWidget->setCurrentIndex(1);
    m_ptUi->setButton->setVisible(false);
}

void Loggin::on_minButton_clicked() {
    this->showMinimized();
}

void Loggin::on_closeButton_clicked() {
    this->close();
}

void Loggin::on_sureSetButton_clicked() {
    // 更新全局的服务器ip
    g_tServerIpAddr = m_ptUi->serverIpEdit->text();
    // 重新连接服务器
    m_ptSocketClient->connectToServer(g_tServerIpAddr);
    // 显示set按键
    m_ptUi->setButton->setVisible(true);
    m_ptUi->stackedWidget->setCurrentIndex(0);
}

void Loggin::on_cancelSetButton_clicked() {
    // 还原之前的值
    m_ptUi->serverIpEdit->setText(g_tServerIpAddr);
    // 显示set按键
    m_ptUi->setButton->setVisible(true);
    m_ptUi->stackedWidget->setCurrentIndex(0);
}

void Loggin::on_logButton_clicked() {
    // 判断数据正确性
    if (0 == m_ptUi->idEdit->text().length() || 0 == m_ptUi->passwordEdit->text().length()) {
        return ;
    }
    // 更新全局的个人信息
    g_tMyselfInfo.sId       = m_ptUi->idEdit->text();
    g_tMyselfInfo.sPassword = m_ptUi->passwordEdit->text();
    // 读取数据库，判断账号密码是否正确
    sendIdAndPassword();
    // 服务器在接到数据之后，若账户和密码正确，则构造UserInfo并发送给客户端
    // 客户端接收到数据之后，调用SocketClient::onReadyRead()解析数据，并赋值给全局的UserInfo
    // 随后调用SocketClient::sigStatus()触发Loggin::onSigStatus()
    // Loggin::onSigStatus()根据服务器数据的状态位进行判断和错误处理

#if 0
    recvUserInfo();
    // 显示好友界面
    m_ptFriend->preInit();
    m_ptFriend->show();
    m_ptFriend->postInit();
    // 关闭登录界面
    this->hide();
#endif
}

void Loggin::on_cancelButton_clicked() {
//    qApp->quit();
    this->close();
}

void Loggin::on_losePasswordButton_clicked() {
    // 显示找回密码界面
    m_ptRecoverPassword = new RecoverPassword();
    m_ptRecoverPassword->show();
    this->hide();
}

void Loggin::on_newButton_clicked() {
    // 显示注册账号界面
    m_ptRegisterId = new RegisterId();
    m_ptRegisterId->show();
    this->hide();
}

void Loggin::onSigStatus(int reType/* const Sst& reType */) {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__ << reType;
#endif
    switch (reType) {
        case SST_CONNECTED: {
            break;
        }
        case SST_DISCONNECTED: {
            break;
        }
        case SST_LOGIN_SUCCESS: {
            // 根据用户ID拉取分组信息
            sendId();
            break;
        }
        case SST_PASSWORD_ERROR: {
            showTipWindow(QStringLiteral("警告"), QStringLiteral("登录失败！\n请检查账号密码是否正确！"));
            break;
        }
        case SST_LOGIN_REPEAT: {
            showTipWindow(QStringLiteral("警告"), QStringLiteral("登录失败！\n此账号当前已经登录！"));
            break;
        }
        case SST_REGISTER_SUCCESS: {
            // 此处不需要更新全局的UserInfo，因为登录账号还没有确定
            // 更新Loggin中的预设账号
            m_ptUi->idEdit->setText(m_ptRegisterId->getIdEditText());
            // 关闭注册界面
            m_ptRegisterId->close();
            this->show();
            break;
        }
        case SST_REGISTER_FAILED: {
            showTipWindow(QStringLiteral("警告"), QStringLiteral("当前账号名称已存在！\n请重新输入！"));
            break;
        }
        case SST_MODIFYPASSWORD_SUCCESS: {
            break;
        }
        case SST_MODIFYPASSWORD_FAILED: {
            showTipWindow(QStringLiteral("错误"), QStringLiteral("初始化失败！\n请重试！"));
            break;
        }
        case SST_GETGROUP_SUCCESS: {
            // 拉取好友信息准备在Friend的preInit()中完成
            // 取消信号连接
//            disconnect(m_ptSocketClient, SIGNAL(sigStatus(int)), this, SLOT(onSigStatus(int)));
            // Friend界面预初始化以及显示
            m_ptFriend->preInit();
            m_ptFriend->show();
            m_ptFriend->postInit();
            // 关闭登录界面
            this->hide();
            break;
        }
        case SST_GETGROUP_FAILED: {
            showTipWindow(QStringLiteral("错误"), QStringLiteral("初始化失败！\n请重试！"));
            break;
        }
    }
}

void Loggin::setServerIpInputFormat(void) {
    QRegExp tRx("((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)");
    m_ptRxv = new QRegExpValidator(tRx);
    m_ptUi->serverIpEdit->setValidator(m_ptRxv);
//    m_ptUi->serverIpEdit->setInputMask(QString("000.000.000.000"));
}

void Loggin::setControlsByConfig(void) {
    m_ptUi->idEdit->setText(g_tMyselfInfo.sId);
    m_ptUi->passwordEdit->setText(g_tMyselfInfo.sPassword);

    m_ptUi->saveBox->setChecked((bool)g_tMyselfInfo.sPassword.length());
    m_ptUi->serverIpEdit->setText(g_tServerIpAddr);
}

void Loggin::setSocketClient(void) {
    m_ptSocketClient = SocketClient::getInstance();
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << __LINE__;
#endif
    m_ptSocketClient->connectToServer(g_tServerIpAddr);
    connect(m_ptSocketClient, SIGNAL(sigStatus(int)), this, SLOT(onSigStatus(int)));
}

void Loggin::sendIdAndPassword(void) {
    QJsonObject tData;
    tData.insert("Id", g_tMyselfInfo.sId);
    tData.insert("Password", g_tMyselfInfo.sPassword);

    m_ptSocketClient->onSendMessage(SMT_LOGIN, tData);
}

void Loggin::sendId(void) {
    QJsonObject tData;
    tData.insert("Id", g_tMyselfInfo.sId);

    m_ptSocketClient->onSendMessage(SMT_GETGROUP, tData);
}

void Loggin::showTipWindow(const QString& rsTitle, const QString& rsTip,
                           const QString& rsButtonText) {
    QMessageBox tBox(QMessageBox::Warning, rsTitle, rsTip);
    tBox.setStandardButtons(QMessageBox::Ok);
    tBox.setButtonText(QMessageBox::Ok, rsButtonText);
    tBox.exec();
}

void Loggin::recvUserInfo(void) {
    // 用于填充或清空UserInfo结构体
#ifdef _DEBUG_STATE
    g_tMyselfInfo.sName = QStringLiteral("张三");
    g_tMyselfInfo.sHead = USER_HEAD_DEFAULT;
#else
    QJsonObject tData;
    tData.insert("Id", g_tMyselfInfo.sId);

    m_ptSocketClient->onRecvMessage(SMT_GETUSERINFO, tData);
#endif
}
