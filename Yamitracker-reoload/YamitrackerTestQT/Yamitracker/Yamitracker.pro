QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QT += core gui widgets multimedia

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

PROJECT_ROOT = $$PWD
BUILD_ROOT = $$PROJECT_ROOT/build


DESTDIR = /$$BUILD_ROOT/BIN
OBJECTS_DIR = /$$BUILD_ROOT/obj
RCC_DIR = /$$BUILD_ROOT/rcc
MOC_DIR = /$$BUILD_ROOT/moc
UI_DIR = /$$BUILD_ROOT/ui


SOURCES += \
    format/brrconverter.cpp \
    main.cpp \
    format/simplewavwriter.cpp \
#    format/spcrecord.cpp \
    src/yamitracker.cpp

HEADERS += \
    format/brrconverter.h \
    format/simplewavwriter.h \
 #   format/spcrecord.h \
    src/yamitracker.h

FORMS += \
    ui/yamitracker.ui

INCLUDEPATH += format
INCLUDEPATH += ui
INCLUDEPATH += src

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
