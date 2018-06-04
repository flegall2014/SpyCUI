#ifndef SPYCLIB_GLOBAL_H
#define SPYCLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SPYCLIB_LIBRARY)
#  define SPYCLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SPYCLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SPYCLIB_GLOBAL_H
