QT += core
QMAKE_CXX = clang++
QMAKE_CC = clang
QMAKE_LINK = clang++
CONFIG += console c++11
TARGET = pedal_tester
TEMPLATE = app

SOURCES += pedal_tester.cpp
HEADERS += pedal_tester.cpp

