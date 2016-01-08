#-------------------------------------------------
#
# Project created by QtCreator 2016-01-07T10:28:42
#
#-------------------------------------------------

QT       -= gui

TARGET = extensionsystem

include(../../framework.pri)

DEFINES += EXTENSIONSYSTEM_LIBRARY

SOURCES += \
    iplugin.cpp \
    pluginmanager.cpp \
    pluginspec.cpp \
    plugincollection.cpp

HEADERS +=\
        extensionsystem_global.h \
    iplugin.h \
    pluginmanager.h \
    pluginspec.h \
    pluginmanager_p.h \
    plugincollection.h \
    pluginspec_p.h \
    iplugin_p.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    extensionsystem_dependencies.pri
