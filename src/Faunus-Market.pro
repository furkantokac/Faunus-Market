#-------------------------------------------------
#
# Project created by QtCreator 2016-08-18T17:12:06
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Faunus-Market
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    httpdownload.cpp

HEADERS  += mainwindow.h \
    httpdownload.h

FORMS    += mainwindow.ui
