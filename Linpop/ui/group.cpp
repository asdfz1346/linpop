
#include <group.h>
#include <ui_group.h>

#include <QMessageBox>

Group::Group(QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::Group) {
    m_ptUi->setupUi(this);
}

Group::~Group() {
    delete m_ptUi;
}

int  Group::pullGroupItemList(/*const UserInfo& rtMyselfInfo*/) {
    g_lsGroupTextList.clear();
#ifdef _USE_SQL
    getGroupListById(g_tMyselfInfo.sId, g_lsGroupTextList);
    if (0 == g_lsGroupTextList.length()) {
        g_lsGroupTextList.append(QStringLiteral(GROUPITEM_NAME_DEFAULT));
    }
#else
    g_lsGroupTextList.append(QStringLiteral("我的好友1"));
    g_lsGroupTextList.append(QStringLiteral("我的好友2"));
    g_lsGroupTextList.append(QStringLiteral("我的好友3"));
    g_lsGroupTextList.append(QStringLiteral("我的好友4"));
#endif
    return g_lsGroupTextList.length();
}

void Group::pushGroupItemList(/*const UserInfo& rtMyselfInfo*/) {

}

void Group::pullFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_lptGroupItem.at(iIndex)->pullFriendItemList();
}

void Group::pushFriendItemList(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_lptGroupItem.at(iIndex)->pushFriendItemList();
}

void Group::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    for (int i = 0; i < g_lsGroupTextList.length(); ++i) {
        GroupItem* ptItem = new GroupItem(i, this);
        m_ptUi->widgetLayout->layout()->addWidget(ptItem);  // 优先显示
        m_lptGroupItem.append(ptItem);
    }
    // 从此处开始，m_lptGroupItem和g_lsGroupTextList的设置都由客户端同步完成
}

void Group::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_lptGroupItem.at(iIndex)->initFriendItemControls(iIndex);
}

void Group::addGroupItem(const QString& rsName) {
    // 在GroupItem的构造函数中使用了index对应的g_lsGroupTextList
    g_lsGroupTextList.append(rsName);

    GroupItem* ptItem = new GroupItem(g_lsGroupTextList.length() - 1, this);
    m_ptUi->widgetLayout->layout()->addWidget(ptItem);      // 优先显示

    m_lptGroupItem.append(ptItem);
}

void Group::delGroupItem(const int iIndex) {
    // 删除分组前必须确定分组个数大于等于两个
    int iDestIndex = findDefaultGroupItem(iIndex);
    if (-1 == iDestIndex) {
        QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除分组"), QStringLiteral("默认分组无法删除！"));
        tBox.setStandardButtons(QMessageBox::Ok);
        tBox.setButtonText(QMessageBox::Ok, QString(QStringLiteral("确定")));
        tBox.exec();
        return ;
    }

    QString sTips = QStringLiteral("删除此分组后，系统会将好友移动至\n默认分组\"");
    sTips.append(g_lsGroupTextList.at(iDestIndex));
    sTips.append(QStringLiteral("\"中"));
    QMessageBox tBox(QMessageBox::Warning, QStringLiteral("删除分组"), sTips);
    tBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    tBox.setButtonText(QMessageBox::Yes, QString(QStringLiteral("确定")));
    tBox.setButtonText(QMessageBox::No,  QString(QStringLiteral("取消")));
    int iRet = tBox.exec();
    if (QMessageBox::No == iRet) {
        return ;
    }

    // 将待删除分组内的好友移动至第一个分组中
    m_lptGroupItem.at(iIndex)->moveAllFriendItems(iDestIndex);

    // 删除分组
    m_lptGroupItem.at(iIndex)->setVisible(false);
    m_ptUi->widgetLayout->layout()->removeWidget(m_lptGroupItem.at(iIndex));

    m_lptGroupItem.removeAt(iIndex);
    g_lsGroupTextList.removeAt(iIndex);

    // 更新GroupItem的index
    for (int i = 0; i < m_lptGroupItem.length(); ++i) {
        m_lptGroupItem.at(i)->setGroupItemIndex(i);
    }
}

GroupItem* Group::getGroupitemIndex(const int iIndex) {
    return m_lptGroupItem.at(iIndex);
}

int  Group::findDefaultGroupItem(const int iIndex) {
    return iIndex ? 0 : -1;
}
