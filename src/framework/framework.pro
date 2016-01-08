TEMPLATE = subdirs

SUBDIRS += \
    extensionsystem \

for(l, SUBDIRS) {
    APP_LIB_DEPENDS =
    include($$l/$${l}_dependencies.pri)
    lv = $${l}.depends
    $$lv = $$APP_LIB_DEPENDS
}
