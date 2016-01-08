#ifndef COREMODULE_GLOBAL_H
#define COREMODULE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COREMODULE_LIBRARY)
#  define COREMODULE_EXPORT Q_DECL_EXPORT
#else
#  define COREMODULE_EXPORT Q_DECL_IMPORT
#endif

#endif // COREMODULE_GLOBAL_H
