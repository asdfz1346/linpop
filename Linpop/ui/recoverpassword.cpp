
#include <recoverpassword.h>
#include <ui_recoverpassword.h>
#include <loggin.h>

RecoverPassword::RecoverPassword(QWidget* ptParent) :
    QWidget(ptParent), m_ptUi(new Ui::RecoverPassword) {
    m_ptUi->setupUi(this);

    this->setWindowFlags(windowFlags() &~Qt::WindowMinMaxButtonsHint);
}

RecoverPassword::~RecoverPassword() {
    delete m_ptUi;
}

void RecoverPassword::showStackPage(const int iIndex) {
    if (iIndex) {
        m_ptUi->stackedWidget->setCurrentIndex(1);
        m_ptUi->sureButton->setText(QStringLiteral("确 定"));
    }
    else {
        on_cancelButton_clicked();
    }
}

void RecoverPassword::on_sureButton_clicked() {
    if (0 == m_ptUi->stackedWidget->currentIndex()) {
        // 验证账号对应的提示信息是否正确
        if (m_ptUi->idEdit->text().length() && m_ptUi->tipEdit->text().length()) {
            Loggin::getInstance()->sendToGetPosswordTip(m_ptUi->idEdit->text(), m_ptUi->tipEdit->text());
        }
        return ;
    }

    if (m_ptUi->passwordEdit->text().length() && m_ptUi->repassEdit->text().length()) {
        Loggin::getInstance()->sendToGetModifyPossword(m_ptUi->idEdit->text(), m_ptUi->passwordEdit->text());
    }
}

void RecoverPassword::on_cancelButton_clicked() {
    m_ptUi->stackedWidget->setCurrentIndex(0);
    m_ptUi->sureButton->setText(QStringLiteral("找 回"));

    Loggin::getInstance()->show();
    this->close();
}
