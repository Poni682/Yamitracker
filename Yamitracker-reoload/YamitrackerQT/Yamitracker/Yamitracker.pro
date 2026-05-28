QT += core gui widgets

CONFIG += c++17

# Явно указываем Qt6
QT_MAJOR_VERSION = 6

SOURCES += \
    main.cpp \
    yamitracker.cpp \
    simplemidiwriter.cpp \
    yamimedia.cpp

HEADERS += \
    yamitracker.h \
    simplemidiwriter.h \
    yamimedia.h

FORMS += \
    yamitracker.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target