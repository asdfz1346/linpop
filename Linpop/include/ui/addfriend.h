
#ifndef LINPOP_ADDFRIEND_H
#define LINPOP_ADDFRIEND_H

#include <common.h>

#include <QWidget>

namespace Ui {
class AddFriend;
}

class AddFriend : public QWidget {
    Q_OBJECT

public:
    explicit AddFriend(const FriendInfo& rtFriendInfo, QWidget* ptParent = nullptr);
    ~AddFriend();

private:
    Ui::AddFriend* m_ptUi;
};

#endif // LINPOP_ADDFRIEND_H
