
#ifndef LINPOP_CLIENTFILE_H
#define LINPOP_CLIENTFILE_H

#include <QString>

#define SETTINGS_USER_GROUP     ("UserConfig")
#define SETTINGS_ID_KEY         ("Id")
#define SETTINGS_PW_KEY         ("Password")
#define SETTINGS_SERVER_GROUP   ("ServerIpAddr")
#define SETTINGS_SERVER_KEY     SETTINGS_SERVER_GROUP

class ClientFile {
public:
    static QString m_sConfigPath;       // 配置文件路径
    static QString m_sIdPath;           // 账户名路径
    static QString m_sDataPath;         // 聊天记录路径
    static QString m_sFilePath;         // 文件路径
    static QString m_sHeadPath;         // 头像路径
    
    static void initClientDir(void);
    static void checkClientDir(void);
    
    static void createConfigFile(void);
    static void readConfigFile(void);
    static void writeConfigFile(bool bIsSavePassword);
    
    static void setConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue);
    static QString getConfigString(const QString& rsGroup, const QString& rsKey, const QString& rtValue);
    
};

#endif // LINPOP_CLIENTFILE_H
