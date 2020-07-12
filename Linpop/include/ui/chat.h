
#ifndef LINPOP_CHAT_H
#define LINPOP_CHAT_H

#include <common.h>

#include <QWidget>

namespace Ui {
class Chat;
}

class Chat : public QWidget {
    Q_OBJECT

public:
    explicit Chat(const FriendPosition& rtPosition, const FriendInfo& rtFriendInfo,
                  QWidget* ptParent = nullptr);
    ~Chat();

private slots:
    void on_sendButton_clicked();

private:
    Ui::Chat*      m_ptUi;

    // 当前位置
    FriendPosition m_tPosition;
};

#endif // LINPOP_CHAT_H
