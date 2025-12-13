QT += core gui widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    yamitracker.cpp \
    simplewavwriter.cpp \
    brrconverter.cpp \
    audiosynthesizer.cpp

HEADERS += \
    yamitracker.h \
    simplewavwriter.h \
    brrconverter.h \
    audiosynthesizer.h

FORMS += \
    yamitracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target