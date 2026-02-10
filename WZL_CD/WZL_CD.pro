QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    createdirdialog.cpp \
    global.cpp \
    httpmgr.cpp \
    if_or_not_remove.cpp \
    logindialog.cpp \
    m_dir.cpp \
    m_file.cpp \
    main.cpp \
    maindialog.cpp \
    mainwindow.cpp \
    newnamedialog.cpp \
    readatathread.cpp \
    registerdialog.cpp \
    tcpmanger.cpp \
    waitdialog.cpp

HEADERS += \
    createdirdialog.h \
    global.h \
    httpmgr.h \
    if_or_not_remove.h \
    logindialog.h \
    m_dir.h \
    m_file.h \
    maindialog.h \
    mainwindow.h \
    newnamedialog.h \
    readatathread.h \
    registerdialog.h \
    tcpmanger.h \
    waitdialog.h

FORMS += \
    createdirdialog.ui \
    if_or_not_remove.ui \
    logindialog.ui \
    m_dir.ui \
    m_file.ui \
    maindialog.ui \
    mainwindow.ui \
    newnamedialog.ui \
    registerdialog.ui \
    waitdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res1.qrc
