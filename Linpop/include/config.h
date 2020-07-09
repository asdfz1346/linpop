
// 避免乱码
//#pragma execution_character_set("utf-8")

#ifndef LINPOP_CONFIG_H
#define LINPOP_CONFIG_H

#define _DEBUG_STATE
//#define _USE_SQL
#define _USE_GROUP_LIST_INIT

#ifdef _DEBUG_STATE
#include <QDebug>
#endif

// 客户端文件夹
#define CLIENT_FILE_DIR         ("File/")
#define CLIENT_DATA_DIR         ("Data/")
#define CLIENT_HEAD_DIR         ("Head/")
#define CLIENT_SET_FILE		("config.ini")

// 服务器相关
#define SOCKET_SERVER_DEFAULT   ("175.24.5.212")
#define SOCKET_PORT_DEFAULT     (12345)

// QT界面相关
#define LOGGIN_SET_FILE_LINES	(3)

#define GROUPITEM_NAME_DEFAULT	QStringLiteral("我的好友")

#define USER_HEAD_DEFAULT       (":/head/6.png")

#endif // LINPOP_CONFIG_H
