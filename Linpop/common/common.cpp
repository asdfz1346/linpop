
#include <common.h>

UserInfo           g_tMyselfInfo;
QString            g_tServerIpAddr;
QMap<int, QString> g_msGroupTextMap;

FriendPosition::FriendPosition(int iGroupIndex, int iFriendIndex) {
    this->iGroupIndex  = iGroupIndex;
    this->iFriendIndex = iFriendIndex;
}

bool operator==(const FriendPosition& rtA, const FriendPosition& rtB) {
    if ((rtA.iFriendIndex == rtB.iFriendIndex) && (rtA.iGroupIndex == rtB.iGroupIndex)) {
        return true;
    }
    return false;
}
