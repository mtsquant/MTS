#ifndef BASE_GLOBAL_H
#define BASE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef BASE_LIB
# define BASE_API Q_DECL_EXPORT
#else
# define BASE_API Q_DECL_IMPORT
#endif

#endif // BASE_GLOBAL_H
