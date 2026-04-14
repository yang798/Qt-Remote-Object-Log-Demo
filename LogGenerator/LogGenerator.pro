QT -= gui
QT += core remoteobjects

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG(debug, debug|release) {
    DESTDIR     = $$PWD/../bin/debug
    OBJECTS_DIR = $$PWD/../build/debug/$$TARGET/obj
    MOC_DIR     = $$PWD/../build/debug/$$TARGET/moc
    UI_DIR      = $$PWD/../build/debug/$$TARGET/ui
    RCC_DIR     = $$PWD/../build/debug/$$TARGET/rcc
} else {
    DESTDIR     = $$PWD/../bin/release
    OBJECTS_DIR = $$PWD/../build/release/$$TARGET/obj
    MOC_DIR     = $$PWD/../build/release/$$TARGET/moc
    UI_DIR      = $$PWD/../build/release/$$TARGET/ui
    RCC_DIR     = $$PWD/../build/release/$$TARGET/rcc
}

SOURCES += \
        LogGenerate.cpp \
        LogRemoteHelper.cpp \
        main.cpp

INCLUDEPATH += $$PWD/../include
REPC_SOURCE = $$PWD/../include/myLog.rep

win32 {
    CONFIG -= console
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../include/LogDef.h \
    LogGenerate.h \
    LogRemoteHelper.h
