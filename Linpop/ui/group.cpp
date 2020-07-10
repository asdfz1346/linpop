
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

void Group::initGroupItemControls(/*const UserInfo& rtMyselfInfo*/) {
    QMap<int, QString>::iterator tIter = g_msGroupTextMap.begin();
    while (tIter != g_msGroupTextMap.end()) {
        GroupItem* ptItem = new GroupItem(tIter.key(), this);
        m_ptUi->widgetLayout->layout()->addWidget(ptItem);  // 优先显示
        m_mptGroupItem.insert(tIter.key(), ptItem);
        ++tIter;
    }
    // 从此处开始，m_lptGroupItem和g_lsGroupTextList的设置都由客户端同步完成
}

void Group::initFriendItemListAppend(const int iIndex, const FriendInfo& rtFriendInfo) {
    m_mptGroupItem[iIndex]->initFriendItemListAppend(rtFriendInfo);
}

void Group::initFriendItemControls(/*const UserInfo& rtMyselfInfo,*/ const int iIndex) {
    m_mptGroupItem[iIndex]->initFriendItemControls();
}

GroupItem* Group::getGroupitemIndex(const int iIndex) {
    return m_mptGroupItem[iIndex];
}

void Group::addGroupItemControls(const int iIndex, const QString& rsName) {
    // 在GroupItem的构造函数中使用了index对应的g_msGroupTextMap
    g_msGroupTextMap.insert(iIndex, rsName);
    GroupItem* ptItem = new GroupItem(iIndex, this);
    m_ptUi->widgetLayout->layout()->addWidget(ptItem);      // 优先显示

    m_mptGroupItem.insert(iIndex, ptItem);
}

void Group::renameGroupItemControls(const int iIndex, const QString& rsName) {
    g_msGroupTextMap[iIndex] = rsName;
    m_mptGroupItem[iIndex]->setGroupItemTextUseCount();
    m_mptGroupItem[iIndex]->showStackPage(0);
}

void Group::delGroupItemControls(const int iSrcGroupIndex, const int iDestGroupIndex) {
    // 将待删除分组内的好友移动至第一个分组中
    m_mptGroupItem[iSrcGroupIndex]->moveAllFriendItems(iDestGroupIndex);
    // 删除分组
    m_mptGroupItem[iSrcGroupIndex]->setVisible(false);
    m_ptUi->widgetLayout->layout()->removeWidget(m_mptGroupItem[iSrcGroupIndex]);

    m_mptGroupItem.remove(iSrcGroupIndex);
    g_msGroupTextMap.remove(iSrcGroupIndex);
}

void Group::addFriendItem(const int iIndex, const FriendInfo& rtFriendInfo){
    m_mptGroupItem[iIndex]->addFriendItem(rtFriendInfo);
}

void Group::moveFriendItem(const int iSrcGroupIndex, const int iDestGroupIndex, const int iIndex) {
    m_mptGroupItem[iSrcGroupIndex]->moveFriendItem(iIndex, iDestGroupIndex);
}

void Group::delFriendItem(const int iGroupIndex, const int iIndex) {
    m_mptGroupItem[iGroupIndex]->delFriendItem(iIndex);
}

int  Group::findDefaultGroupItem(const int iIndex) {
    return iIndex ? 0 : -1;
}
