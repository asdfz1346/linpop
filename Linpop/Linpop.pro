#-------------------------------------------------
#
# Project created by QtCreator 2020-07-02T15:45:51
#
#-------------------------------------------------

QT      += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET   = Linpop
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG  += c++11

QMAKE_CXXFLAGS_WARN_ON += -wd4819       # 屏蔽 c4819 warning

SOURCES += \
    common/common.cpp \
    common/clientfile.cpp \
    common/singleton.cpp \
    anti/socketclient.cpp \
    ui/moveableframelesswindow.cpp \
    ui/loggin.cpp \
    ui/recoverpassword.cpp \
    ui/registerid.cpp \
    ui/friend.cpp \
    ui/group.cpp \
    ui/groupitem.cpp \
    ui/frienditem.cpp \
    main.cpp

INCLUDEPATH += \
    include/ \
    include/common/ \
    include/anti/ \
    include/ui/

HEADERS += \
    include/anti/socketclient.h \
    include/ui/moveableframelesswindow.h \
    include/ui/loggin.h \
    include/ui/recoverpassword.h \
    include/ui/registerid.h \
    include/ui/friend.h \
    include/ui/group.h \
    include/ui/groupitem.h \
    include/ui/frienditem.h

FORMS += \
    ui/loggin.ui \
    ui/recoverpassword.ui \
    ui/registerid.ui \
    ui/friend.ui \
    ui/group.ui \
    ui/groupitem.ui \
    ui/frienditem.ui

RESOURCES += \
    resource/qt.qrc
