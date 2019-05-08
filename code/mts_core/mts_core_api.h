
/*****************************************************************************
* Copyright [2017-2019] [MTSQuant]
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*  http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*****************************************************************************/
#ifndef MTS_CORE_GLOBAL_H
#define MTS_CORE_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MTS_CORE_LIB
# define MTS_CORE_API Q_DECL_EXPORT
#else
# define MTS_CORE_API Q_DECL_IMPORT
#endif

#endif // MTS_CORE_GLOBAL_H
