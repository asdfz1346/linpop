
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
    m_ptSocketClient = new SocketClient(this);
}

Loggin::~Loggin() {
#ifdef _DEBUG_STATE
    qDebug() << __FUNCTION__ << " : " << __LINE__;
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

    delete m_ptSocketClient;
    delete m_ptUi;
}

void Loggin::setEditText(const QString& rsId, const QString& rsPassword) {
    m_ptUi->idEdit->setText(rsId);
    m_ptUi->passwordEdit->setText(rsPassword);
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
    recvUserInfo();
    if (0 == g_tMyselfInfo.sId.length()) {
        QMessageBox tBox(QMessageBox::Warning, QStringLiteral("警告"),
                         QStringLiteral("登录失败！\n请检查账号密码是否正确！"));
        tBox.setStandardButtons(QMessageBox::Ok);
        tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
        tBox.exec();
        return ;
    }

    // 显示好友界面
    m_ptFriend->preInit();
    m_ptFriend->show();
    m_ptFriend->postInit();
    // 关闭登录界面
    this->hide();
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

void Loggin::sendIdAndPassword(void) {
    QJsonObject tData;
    tData.insert("Id", g_tMyselfInfo.sId);
    tData.insert("Password", g_tMyselfInfo.sPassword);

    m_ptSocketClient->onSendMessage(Login, tData);
}

void Loggin::recvUserInfo(void) {
    // 用于填充或清空UserInfo结构体
#ifdef _DEBUG_STATE
    g_tMyselfInfo.sName = QStringLiteral("张三");
    g_tMyselfInfo.sIcon = FRIEND_ICON_DEFAULT;
#else

#endif
}
