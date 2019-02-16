
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
#ifndef MTS_AGENT_PYTHON_GLOBAL_H
#define MTS_AGENT_PYTHON_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef MTS_AGENT_PYTHON_LIB
# define MTS_AGENT_PYTHON_API Q_DECL_EXPORT
#elif defined MTS_UNIT_TEST
#define MTS_AGENT_PYTHON_API 
#else
# define MTS_AGENT_PYTHON_API Q_DECL_IMPORT
#endif

#endif // MTS_AGENT_PYTHON_GLOBAL_H
