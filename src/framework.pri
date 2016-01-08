depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$depfile) {
    include($$depfile)
    isEmpty(APP_LIB_NAME): \
        error("$$basename(depfile) does not define APP_LIB_NAME.")
} else {
    isEmpty(APP_LIB_NAME): \
        error("APP_LIB_NAME is empty. Maybe you meant to create $$basename(depfile)?")
}
TARGET = $$APP_LIB_NAME

include(../gc_framework.pri)

win32 {
    DLLDESTDIR = $$APP_PATH
}

DESTDIR = $$APP_LIBRARY_PATH

TARGET = $$qtLibraryName($$TARGET)

TEMPLATE = lib
CONFIG += shared dll
