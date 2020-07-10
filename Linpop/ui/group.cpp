
#include <group.h>
#include <ui_group.h>
#include <friend.h>

#include <QMessageBox>

Group::Group(QWidget* ptParent) : QWidget(ptParent), m_ptUi(new Ui::Group) {
    m_ptUi->setupUi(this);
}

Group::~Group() {
    delete m_ptUi;
}
#if 0
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
#endif

void Group::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    for (int i = 0; i < g_lsGroupTextList.length(); ++i) {
        GroupItem* ptItem = new GroupItem(i, this);
        m_ptUi->widgetLayout->layout()->addWidget(ptItem);  // 优先显示
        m_lptGroupItem.append(ptItem);
    }
    // 从此处开始，m_lptGroupItem和g_lsGroupTextList的设置都由客户端同步完成
}

void Group::initFriendItemListAppend(const int iIndex, const FriendInfo& rtFriendInfo) {
    m_lptGroupItem.at(iIndex)->initFriendItemListAppend(rtFriendInfo);
}

void Group::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_lptGroupItem.at(iIndex)->initFriendItemControls(iIndex);
}

GroupItem* Group::getGroupitemIndex(const int iIndex) {
    return m_lptGroupItem.at(iIndex);
}

void Group::addGroupItemControls(const QString& rsName) {
    // 在GroupItem的构造函数中使用了index对应的g_lsGroupTextList
    g_lsGroupTextList.append(rsName);
    GroupItem* ptItem = new GroupItem(g_lsGroupTextList.length() - 1, this);
    m_ptUi->widgetLayout->layout()->addWidget(ptItem);      // 优先显示

    m_lptGroupItem.append(ptItem);
}

void Group::renameGroupItemControls(const int iIndex, const QString& rsName) {
    g_lsGroupTextList.replace(iIndex, rsName);
    m_lptGroupItem.at(iIndex)->setGroupItemTextUseCount();
    m_lptGroupItem.at(iIndex)->showStackPage(0);
}

void Group::delGroupItemControls(const int iSrcGroupIndex, const int iDestGroupIndex) {
    // 将待删除分组内的好友移动至第一个分组中
    m_lptGroupItem.at(iSrcGroupIndex)->moveAllFriendItems(iDestGroupIndex);
    // 删除分组
    m_lptGroupItem.at(iSrcGroupIndex)->setVisible(false);
    m_ptUi->widgetLayout->layout()->removeWidget(m_lptGroupItem.at(iSrcGroupIndex));

    m_lptGroupItem.removeAt(iSrcGroupIndex);
    g_lsGroupTextList.removeAt(iSrcGroupIndex);

    // 更新GroupItem的index
    for (int i = 0; i < m_lptGroupItem.length(); ++i) {
        m_lptGroupItem.at(i)->setGroupItemIndex(i);
    }
}

void Group::addFriendItem(const int iGroupIndex, const FriendInfo& rtFriendInfo){
    m_lptGroupItem.at(iGroupIndex)->addFriendItem(rtFriendInfo);
}

void Group::moveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const int iIndex) {
    m_lptGroupItem.at(iSrcGroupIndex)->moveFriendItem(iIndex, iDestGroupIndex);
}

void Group::delFriendItem(const int iGroupIndex, const int iIndex) {
    m_lptGroupItem.at(iGroupIndex)->delFriendItem(iIndex);
}

int  Group::findDefaultGroupItem(const int iIndex) {
    return iIndex ? 0 : -1;
}
