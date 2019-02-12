/*****************************************************************************
* Copyright [2018-2019] [3fellows]
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
#ifndef CTP_EXT_GLOBAL_H
#define CTP_EXT_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef CTP_EXT_LIB
# define CTP_EXT_API Q_DECL_EXPORT
#else
# define CTP_EXT_API Q_DECL_IMPORT
#endif

#endif // CTP_EXT_GLOBAL_H
