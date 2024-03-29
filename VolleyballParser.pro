#-------------------------------------------------
#
# Project created by QtCreator 2017-10-14T10:07:24
#
#-------------------------------------------------


QT       += core gui xml
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = VolleyballParser
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    StatCrewView.cpp \
    VolleyballPlayer.cpp \
    StatCrewReader.cpp \
    EspnVolleyball.cpp \
    MainUi.cpp

HEADERS  += MainWindow.h \
    StatCrewView.h \
    VolleyballPlayer.h \
    StatCrewReader.h \
    EspnVolleyball.h \
    MainUi.h
