#-------------------------------------------------
#
# Project created by QtCreator 2016-07-06T12:21:39
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Faunus-Market
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    downloader.cpp

HEADERS  += mainwindow.h \
    downloader.h

FORMS    += mainwindow.ui
