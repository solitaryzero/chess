#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T14:55:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = chess
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chessboard.cpp \
    chessdata.cpp \
    hostwindow.cpp \
    guestwindow.cpp

HEADERS  += mainwindow.h \
    chessboard.h \
    chessdata.h \
    hostwindow.h \
    guestwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    pic/scs.qrc
