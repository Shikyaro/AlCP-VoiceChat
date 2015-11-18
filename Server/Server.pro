#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T08:43:15
#
#-------------------------------------------------

QT       += core gui declarative network multimedia sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    sclient.cpp \
    database.cpp

HEADERS  += mainwindow.h \
    server.h \
    sclient.h \
    database.h
