
#include <registerid.h>
#include <ui_registerid.h>
#include <loggin.h>

#include <QMessageBox>

RegisterId::RegisterId(QWidget *ptParent) : QWidget(ptParent), m_ptUi(new Ui::RegisterId) {
    m_ptUi->setupUi(this);
}

RegisterId::~RegisterId() {
    delete m_ptUi;
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
        // 判断账号是否存在
#ifdef _USE_SQL
        if (isIdExist(m_ptUi->idEdit->text())) {
            QMessageBox tBox(QMessageBox::Warning, QStringLiteral("警告"),
                             QStringLiteral("当前账号名称已存在！\n请重新输入！"));
            tBox.setStandardButtons(QMessageBox::Ok);
            tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
            tBox.exec();
            return ;
        }
#endif
        // 更新Loggin中的账号
        Loggin* ptLoggin = Loggin::getInstance();
        ptLoggin->setEditText(m_ptUi->idEdit->text(), ""/* m_ptUi->passwordEdit->text() */);
        ptLoggin->show();
        this->close();
    }
}

void RegisterId::on_cancelButton_clicked() {
    Loggin* ptLoggin = Loggin::getInstance();
    ptLoggin->show();
    this->close();
}

void RegisterId::closeEvent(QCloseEvent* ptEvent) {
    Q_UNUSED(ptEvent);
    on_cancelButton_clicked();
}
