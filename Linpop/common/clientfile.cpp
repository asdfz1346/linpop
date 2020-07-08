
#include <clientfile.h>
#include <config.h>
#include <common.h>
#include <loggin.h>

#include <QString>
#include <QSettings>

QString ClientFile::m_sIdPath     = "";                 // 账户名路径
QString ClientFile::m_sDataPath   = "";                 // 聊天记录路径
QString ClientFile::m_sFilePath   = "";                 // 文件路径
QString ClientFile::m_sHeadPath   = "";                 // 头像路径
QString ClientFile::m_sConfigPath = CLIENT_SET_FILE;    // 配置文件路径

void ClientFile::initClientDir(void) {
    // 读取用户名和密码
    
    
    // 
    
    // 注意，点击登录按键后需要更新SETTINGS_RM_KEY
    // 点击设置服务器地址时需要更新SETTINGS_SERVER_KEY
}

void ClientFile::checkClientDir(void) {
    
}

void ClientFile::createConfigFile(void) {
    QSettings tSet(m_sConfigPath, QSettings::IniFormat);
    QString   sGroups = tSet.childGroups().join("");
    
    if (!QFile::exists(m_sConfigPath) || (sGroups.isEmpty())) {
        tSet.beginGroup(SETTINGS_USER_GROUP);
        tSet.setValue(SETTINGS_ID_KEY, "");
        tSet.setValue(SETTINGS_PW_KEY, "");
        tSet.endGroup();

        tSet.beginGroup(SETTINGS_SERVER_GROUP);
        tSet.setValue(SETTINGS_SERVER_KEY, SOCKET_SERVER_DEFAULT);
        tSet.endGroup();
        tSet.sync();
    }
}

void ClientFile::readConfigFile(void) {
    QSettings tSet(m_sConfigPath, QSettings::IniFormat);
    tSet.setIniCodec("UTF8");
    tSet.beginGroup(SETTINGS_USER_GROUP);
    g_tMyselfInfo.sId       = tSet.value(SETTINGS_ID_KEY).toString();
    g_tMyselfInfo.sPassword = tSet.value(SETTINGS_PW_KEY).toString();
    tSet.endGroup();

    tSet.beginGroup(SETTINGS_SERVER_GROUP);
    g_tServerIpAddr = tSet.value(SETTINGS_SERVER_KEY).toString();
    tSet.endGroup();
}

void ClientFile::writeConfigFile(bool bIsSavePassword) {
    QSettings tSet(m_sConfigPath, QSettings::IniFormat);

    tSet.beginGroup(SETTINGS_USER_GROUP);
    tSet.setValue(SETTINGS_ID_KEY, g_tMyselfInfo.sId);
    if (bIsSavePassword) {
        tSet.setValue(SETTINGS_PW_KEY, g_tMyselfInfo.sPassword);
    }
    else {
        tSet.setValue(SETTINGS_PW_KEY, "");
    }
    tSet.endGroup();

    tSet.beginGroup(SETTINGS_SERVER_GROUP);
    tSet.setValue(SETTINGS_SERVER_KEY, g_tServerIpAddr);
    tSet.endGroup();
    tSet.sync();
}

void ClientFile::setConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue) {
    QSettings tSet(m_sConfigPath, QSettings::IniFormat);
    tSet.beginGroup(rsGroup);
    tSet.setValue(rsKey, rtValue);
    tSet.sync();
}

QString ClientFile::getConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue) {
    QSettings tSet(m_sConfigPath, QSettings::IniFormat);
    tSet.beginGroup(rsGroup);
    return tSet.value(rsKey, rtValue).toString();
}
