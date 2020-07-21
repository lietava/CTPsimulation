QT += core
QT -= gui

CONFIG += c++11

TARGET = TrigSim
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    allsim.cpp

HEADERS += \
    allsim.h
