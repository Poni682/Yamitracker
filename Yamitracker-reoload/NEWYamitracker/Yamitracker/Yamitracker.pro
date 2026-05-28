QT       += core gui multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Добавляем пути для include
INCLUDEPATH += /usr/include/rtmidi
INCLUDEPATH += src
INCLUDEPATH += converter
INCLUDEPATH += format/midi
INCLUDEPATH += detected
INCLUDEPATH += style/qcss
INCLUDEPATH += style/qt
INCLUDEPATH += src/groupButton
INCLUDEPATH += ui

LIBS += -lrtmidi -lasound

CONFIG += c++17

SOURCES += \
    detected/detecteddevaice.cpp \
    main.cpp \
    src/groupButton/buttonGroupY.cpp \
    src/yamitracker.cpp \
    style/qt/styleButton.cpp

HEADERS += \
    detected/detecteddevaice.h \
    format/midi/Midi.h \
    format/midi/midiEventList.h \
    src/groupButton/buttonGroupY.h \
    src/yamitracker.h \
    style/qcss/styleListButton.h \
    style/qt/styleButton.h

FORMS += \
    ui/yamitracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
