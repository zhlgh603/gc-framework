!isEmpty(APP_FRAMEWORK_PRI_INCLUDED):error("APP_framework.pri already included")
APP_FRAMEWORK_PRI_INCLUDED=1


defineReplace(qtLibraryName) {
   unset(LIBRARY_NAME)
   LIBRARY_NAME = $$1
   CONFIG(debug, debug|release) {
      !debug_and_release|build_pass {
          mac:RET = $$member(LIBRARY_NAME, 0)_debug
              else:win32:RET = $$member(LIBRARY_NAME, 0)d
      }
   }
   isEmpty(RET):RET = $$LIBRARY_NAME
   return($$RET)
}

APP_SOURCE_TREE=$$PWD

INCLUDEPATH += \
    $$APP_SOURCE_TREE/src/framework \

qt {
    contains(QT, gui): greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
}

isEmpty(APP_BUILD_TREE) {
    sub_dir = $$_PRO_FILE_PWD_
    sub_dir ~= s,^$$re_escape($$PWD),,
    APP_BUILD_TREE = $$clean_path($$OUT_PWD)
    APP_BUILD_TREE ~= s,$$re_escape($$sub_dir)$,,
}

APP_PATH = $$APP_BUILD_TREE/bin

APP_TARGET = E40

isEmpty(APP_OUTPUT_PATH): APP_OUTPUT_PATH = $$APP_BUILD_TREE

APP_BIN_PATH        = $$APP_OUTPUT_PATH/bin
APP_LIBRARY_PATH    = $$APP_OUTPUT_PATH/bin
APP_PLUGIN_PATH     = $$APP_OUTPUT_PATH/plugins

LIBS *= -L$$APP_LIBRARY_PATH

APP_PLUGIN_DIRS += $$APP_SOURCE_TREE/src/module
for(dir, APP_PLUGIN_DIRS) {
    INCLUDEPATH += $$dir
}

# recursively resolve plugin deps
done_plugins =
for(ever) {
    isEmpty(APP_PLUGIN_DEPENDS): \
        break()
    done_plugins += $$APP_PLUGIN_DEPENDS
    for(dep, APP_PLUGIN_DEPENDS) {
        dependencies_file =
        for(dir, APP_PLUGIN_DIRS) {
            exists($$dir/$$dep/$${dep}_dependencies.pri) {
                dependencies_file = $$dir/$$dep/$${dep}_dependencies.pri
                break()
            }
        }
        isEmpty(dependencies_file): \
            error("Plugin dependency $$dep not found")
        include($$dependencies_file)
        LIBS += -l$$qtLibraryName($$APP_PLUGIN_NAME)
    }
    APP_PLUGIN_DEPENDS = $$unique(APP_PLUGIN_DEPENDS)
    APP_PLUGIN_DEPENDS -= $$unique(done_plugins)
}

# recursively resolve library deps
done_libs =
for(ever) {
    isEmpty(APP_LIB_DEPENDS): \
        break()
    done_libs += $$APP_LIB_DEPENDS
    for(dep, APP_LIB_DEPENDS) {
        include($$PWD/src/framework/$$dep/$${dep}_dependencies.pri)
        LIBS += -l$$qtLibraryName($$APP_LIB_NAME)
    }
    APP_LIB_DEPENDS = $$unique(APP_LIB_DEPENDS)
    APP_LIB_DEPENDS -= $$unique(done_libs)
}

