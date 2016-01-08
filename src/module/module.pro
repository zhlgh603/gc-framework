TEMPLATE = subdirs

SUBDIRS += \
    coremodule \

for(l, SUBDIRS) {
    APP_PLUGIN_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$APP_PLUGIN_DEPENDS
}

