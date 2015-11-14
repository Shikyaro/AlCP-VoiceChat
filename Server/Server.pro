#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T08:43:15
#
#-------------------------------------------------

QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Server
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    server.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    server.h \
    client.h
