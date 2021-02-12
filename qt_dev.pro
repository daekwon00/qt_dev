QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

win32{
    #enter your Windows definitions here ...
    DEFINES += QT_WINDOWS_PATH
}
unix{
    macx {
        # Mac OS X definitions should be entered here ...
    }
    else {
        # Linux definitions go here ...
        DEFINES += QT_LINUX_PATH
    }
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    form_sub.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    form_sub.h \
    mainwindow.h

FORMS += \
    form_sub.ui \
    mainwindow.ui

TRANSLATIONS += \
    qt_dev_ko_KR.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qt_dev.qrc
