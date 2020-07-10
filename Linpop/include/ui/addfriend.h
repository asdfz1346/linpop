
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
    explicit AddFriend(const int iGroupIndex, QWidget* ptParent = nullptr);
    ~AddFriend();

    void showStackPage(const int iIndex, const FriendInfo& rtFriendInfo);
    void setGroupIndex(const int iIndex);

private slots:
    void on_sureButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::AddFriend* m_ptUi;
    int            m_iGroupIndex;
};

#endif // LINPOP_ADDFRIEND_H
