TARGET = Yamitraker

QT += core gui widgets multimedia

PROJECT_ROOT = $$PWD
BUILD_ROOT = $$PROJECT_ROOT/build


CONFIG += c++17

DESTDIR = /$$BUILD_ROOT/BIN
OBJECTS_DIR = /$$BUILD_ROOT/obj
RCC_DIR = /$$BUILD_ROOT/rcc
MOC_DIR = /$$BUILD_ROOT/moc
UI_DIR = /$$BUILD_ROOT/ui

SOURCES += \
    main.cpp \
    src/yamitracker.cpp \
    format/simplewavwriter.cpp \
    format/brrconverter.cpp

HEADERS += \
    src/yamitracker.h \
    format/simplewavwriter.h \

INCLUDEPATH += format
INCLUDEPATH += ui
INCLUDEPATH += src

FORMS += \
    ui/yamitracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target