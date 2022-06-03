QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    blockchain.cpp \
    cryptography.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    blockchain.h \
    cryptography.h \
    mainwindow.h \
    pch.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    Blockchain_ru_RU.ts
CONFIG += lrelease
CONFIG += embed_translations

INCLUDEPATH += $$PWD//cryptopp

    !contains(QMAKE_TARGET.arch, x86_64) {
        message("x86 build")

    LIBS += -L$$PWD/cryptopp/Win32/Output/Release -lcryptlib
    }
    else
    {
        message("x86_64 build")

    LIBS += -L$$PWD/cryptopp/x64/Output/Release -lcryptlib
    }



# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
