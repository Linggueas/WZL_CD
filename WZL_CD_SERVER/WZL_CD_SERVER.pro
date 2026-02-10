QT       += core gui network sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



SOURCES += \
    dbmgr.cpp \
    global.cpp \
    log.cpp \
    main.cpp \
    mainwindow.cpp \
    readdatathread.cpp \
    tcpmanger.cpp

HEADERS += \
    dbmgr.h \
    global.h \
    log.h \
    mainwindow.h \
    readdatathread.h \
    tcpmanger.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    WZL_CD_SERVER_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
