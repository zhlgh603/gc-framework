depfile = $$replace(_PRO_FILE_PWD_, ([^/]+$), \\1/\\1_dependencies.pri)
exists($$depfile) {
    include($$depfile)
    isEmpty(APP_PLUGIN_NAME): \
        error("$$basename(depfile) does not define APP_PLUGIN_NAME.")
} else {
    isEmpty(APP_PLUGIN_NAME): \
        error("APP_PLUGIN_NAME is empty. Maybe you meant to create $$basename(depfile)?")
}
TARGET = $$APP_PLUGIN_NAME

plugin_deps = $$APP_PLUGIN_DEPENDS
#plugin_test_deps = $$APP_TEST_DEPENDS
#plugin_recmds = $$APP_PLUGIN_RECOMMENDS

include(../gc_framework.pri)

defineReplace(dependencyName) {
    dependencies_file =
    for(dir, APP_PLUGIN_DIRS) {
        exists($$dir/$$1/$${1}_dependencies.pri) {
            dependencies_file = $$dir/$$1/$${1}_dependencies.pri
            break()
        }
    }
    isEmpty(dependencies_file): \
        error("Plugin dependency $$dep not found")
    include($$dependencies_file)
    return($$APP_PLUGIN_NAME)
}


DESTDIR = $$APP_PLUGIN_PATH

LIBS += -L$$DESTDIR
INCLUDEPATH += $$OUT_PWD

# copy the plugin spec
isEmpty(TARGET) {
    error("module.pri: You must provide a TARGET")
}

TEMPLATE = lib
CONFIG += plugin plugin_with_soname
linux*:QMAKE_LFLAGS += $$QMAKE_LFLAGS_NOUNDEF

TARGET = $$qtLibraryName($$TARGET)

