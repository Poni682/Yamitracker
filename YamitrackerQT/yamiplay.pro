TEMPLATE = app
TARGET = yami_test
CONFIG += console c++11
CONFIG -= app_bundle qt

SOURCES += testPlayYami.cpp \
           yamiplay.cpp

LIBS += -lasound -lm

HEADERS += \
    yamiplay.h