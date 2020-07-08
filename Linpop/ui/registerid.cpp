
#include <registerid.h>
#include <ui_registerid.h>

#include <QMessageBox>

RegisterId::RegisterId(QWidget *ptParent) : QWidget(ptParent), m_ptUi(new Ui::RegisterId) {
    m_ptUi->setupUi(this);

    this->setWindowFlags(windowFlags() &~Qt::WindowMinMaxButtonsHint);

    m_ptLoggin = Loggin::getInstance();
}

RegisterId::~RegisterId() {
    delete m_ptUi;
}

QString RegisterId::getIdEditText(void) {
    return m_ptUi->idEdit->text();
}

void RegisterId::on_logButton_clicked() {
    // 判断数据有效性
    if (m_ptUi->idEdit->text().length()     && m_ptUi->passwordEdit->text().length() &&
        m_ptUi->repassEdit->text().length() && m_ptUi->tipEdit->text().length()      &&
        m_ptUi->nameEdit->text().length()) {
        // 判断两次密码输入是否一致
        if (m_ptUi->passwordEdit->text() != m_ptUi->repassEdit->text()) {
            QMessageBox tBox(QMessageBox::Warning, QStringLiteral("警告"),
                             QStringLiteral("两次输入的密码不一致！\n请重新输入！"));
            tBox.setStandardButtons(QMessageBox::Ok);
            tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
            tBox.exec();
            return ;
        }

        // 将信息发送给数据库
        m_ptLoggin->sendRegisterInfo(m_ptUi->idEdit->text(),  m_ptUi->passwordEdit->text(),
                                     m_ptUi->tipEdit->text(), m_ptUi->nameEdit->text());
        // 后续所有操作都放到接受事件中完成
        // 后续操作包括：判断是否注册成功、向Loggin界面传递文本框内容、关闭本界面
    }
}

void RegisterId::on_cancelButton_clicked() {
    m_ptLoggin->show();
    this->close();
}

void RegisterId::closeEvent(QCloseEvent* ptEvent) {
    Q_UNUSED(ptEvent);
    on_cancelButton_clicked();
}
