#ifndef MTS_CLOCK_REAL_GLOBAL_H
#define MTS_CLOCK_REAL_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MTS_CLOCK_REAL_LIB
# define MTS_CLOCK_REAL_API Q_DECL_EXPORT
#else
# define MTS_CLOCK_REAL_API Q_DECL_IMPORT
#endif

#endif // MTS_CLOCK_REAL_GLOBAL_H
