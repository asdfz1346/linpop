
#ifndef LINPOP_CLIENTFILE_H
#define LINPOP_CLIENTFILE_H

#include <QString>

// 配置文件
#define SETTINGS_USER_GROUP     ("UserConfig")
#define SETTINGS_ID_KEY         ("Id")
#define SETTINGS_PW_KEY         ("Password")
#define SETTINGS_SERVER_GROUP   ("ServerIpAddr")
#define SETTINGS_SERVER_KEY     SETTINGS_SERVER_GROUP

// 个人文件夹
#define DIRPATH_FILE_REAR       ("File/")

class ClientFile {
public:
    static void checkClientDir(void);
    
    static void createConfigFile(void);
    static void readConfigFile(void);
    static void writeConfigFile(bool bIsSavePassword);
    
    static void setConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue);
    static QString getConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue);
};

#endif // LINPOP_CLIENTFILE_H
