
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

void RecoverPassword::on_sureButton_clicked() {
    if (0 == m_ptUi->stackedWidget->currentIndex()) {
        // 验证账号对应的提示信息是否正确
        if (true/* 应为验证账号信息 */) {
            m_ptUi->stackedWidget->setCurrentIndex(1);
            m_ptUi->sureButton->setText(QStringLiteral("确 定"));
        }
        return ;
    }

    // 输入密码之后的事件
    // 更新数据库中的密码
    // 退出显示
    on_cancelButton_clicked();
}

void RecoverPassword::on_cancelButton_clicked() {
    m_ptUi->stackedWidget->setCurrentIndex(0);
    m_ptUi->sureButton->setText(QStringLiteral("找 回"));

    Loggin* ptLoggin = Loggin::getInstance();
    ptLoggin->show();
    this->close();
}
