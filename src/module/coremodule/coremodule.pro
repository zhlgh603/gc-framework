QT       -= gui

TARGET = coremodule

include(../../module.pri)

DEFINES += COREMODULE_LIBRARY

SOURCES += coremodule.cpp

HEADERS += coremodule.h\
        coremodule_global.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

DISTFILES += \
    coremodule_dependencies.pri \
    CoreModule.json

