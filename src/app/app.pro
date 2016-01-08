#-------------------------------------------------
#
# Project created by QtCreator 2016-01-07T10:25:43
#
#-------------------------------------------------

QT       += core gui

include(../../gc_framework.pri)

TEMPLATE = app

TARGET = $$APP_TARGET
DESTDIR = $$APP_PATH


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

LIBS *= -l$$qtLibraryName(ExtensionSystem)
