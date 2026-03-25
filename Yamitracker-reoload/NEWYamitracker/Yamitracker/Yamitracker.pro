QT       += core gui multimedia serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Добавляем пути для include
INCLUDEPATH += /usr/include/rtmidi
INCLUDEPATH += src
INCLUDEPATH += converter
INCLUDEPATH += format/midi
INCLUDEPATH += detected

LIBS += -lrtmidi -lasound

CONFIG += c++17

SOURCES += \
    converter/csvrecorder.cpp \
    detected/detecteddevaice.cpp \
    main.cpp \
    format/midi/midireader.cpp \
    format/midi/midirecorder.cpp \
    src/yamitracker.cpp

HEADERS += \
    converter/csvrecorder.h \
    detected/detecteddevaice.h \
    format/midi/MidiEvent.h \
    format/midi/midireader.h \
    format/midi/midirecorder.h \
    src/yamitracker.h

FORMS += \
    ui/yamitracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
