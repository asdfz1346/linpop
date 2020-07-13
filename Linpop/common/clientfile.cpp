
#include <clientfile.h>
#include <config.h>
#include <common.h>
#include <loggin.h>

#include <QDir>
#include <QString>
#include <QSettings>

void ClientFile::checkClientDir(void) {
    // 个人根文件夹
    QDir tDir;
    // 接收文件目录
    if (!tDir.exists(CLIENT_FILE_DIR)) {
        tDir.mkdir(CLIENT_FILE_DIR);
#ifdef Q_WS_QWS
        QProcess::execute("sync");
#endif
    }
}

void ClientFile::createConfigFile(void) {
    QSettings tSet(CLIENT_SET_FILE, QSettings::IniFormat);
    QString   sGroups = tSet.childGroups().join("");
    
    if (!QFile::exists(CLIENT_SET_FILE) || (sGroups.isEmpty())) {
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
    QSettings tSet(CLIENT_SET_FILE, QSettings::IniFormat);
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
    QSettings tSet(CLIENT_SET_FILE, QSettings::IniFormat);

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
    QSettings tSet(CLIENT_SET_FILE, QSettings::IniFormat);
    tSet.beginGroup(rsGroup);
    tSet.setValue(rsKey, rtValue);
    tSet.sync();
}

QString ClientFile::getConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue) {
    QSettings tSet(CLIENT_SET_FILE, QSettings::IniFormat);
    tSet.beginGroup(rsGroup);
    return tSet.value(rsKey, rtValue).toString();
}
