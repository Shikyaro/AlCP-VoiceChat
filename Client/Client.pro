#-------------------------------------------------
#
# Project created by QtCreator 2015-11-10T08:43:35
#
#-------------------------------------------------

QT       += core gui multimedia network sql


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    audiooutput.cpp \
    client.cpp \
    audioinput.cpp \
    logindialog.cpp

HEADERS  += mainwindow.h \
    audiooutput.h \
    client.h \
    audioinput.h \
    logindialog.h

RESOURCES += \
    smiles.qrc
